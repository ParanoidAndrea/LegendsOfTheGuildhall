# Network Debugging Playbook (for Legends of the Guildhall)

When unexpected bugs happen in a 1v1 networked game, the fastest way to debug is to use a **repeatable checklist** instead of random trial-and-error.

This playbook is tailored to the current architecture in this repo (event-driven gameplay + `NetSystem` send/receive + per-player event names).

## 1) Always reproduce in a deterministic setup

Start with a clean two-process run every time:

1. Start server config (`Run/ReleaseServer.bat`)
2. Start client config (`Run/ReleaseClient.bat`)

Those batch files pass `LoadGameConfig` with server/client XMLs, so both sides boot with explicit `netMode`. Keep using this path while debugging to reduce environment drift.

## 2) Triage bugs by category first

Before changing code, classify the symptom:

- **Desync bug**: both windows continue running, but game states differ.
- **Lost-action bug**: one player's action never appears on the other side.
- **Ordering bug**: action appears, but in the wrong phase/order.
- **Authority bug**: both peers apply an action (double-apply), or nobody applies it.
- **UI-only bug**: network state is correct, but one side renders old state.

This classification tells you where to instrument first.

## 3) Use the event name as your "packet ID"

This codebase sends gameplay intent with string event names such as:

- `StartTurnPlayerX`
- `EndDrawPhrasePlayerX`
- `DiscardCardPlayerX`
- `StealCardPlayerX`
- `VictoryPlayerX`

A practical rule:

> For every networked gameplay action, verify **exactly one send** and **exactly one matching local event handling** on each side.

If that invariant fails, you'll usually find the bug quickly.

## 4) Instrument send/receive/apply in 3 points

For each suspicious action, add temporary logs at:

1. **Before `g_netSystem->Send(...)`**
2. **When the event callback is entered**
3. **After state mutation completes**

Log this minimum payload each time:

- Frame/time
- Event name (full string)
- Local player index and target player index
- Current phrase/state (initial/judgement/draw/action/discard/end)
- `g_netState` and `IsMainPlayer()` result

This makes it obvious whether the bug is:

- Not sent
- Sent but not received
- Received but rejected by state checks
- Applied twice

## 5) Guard against the most common 1v1 networking mistakes

### A) Double execution (local + remote both applying)

Pattern to audit:

- You call `FireEvent(...)` locally
- You also call `g_netSystem->Send(...)`
- Remote side also calls local `FireEvent(...)` with the same condition

Check all event paths that gate on:

- `g_netState == NetState::REMOTE`
- `IsMainPlayer()`

A mismatch in those guards is a classic source of duplicate card plays or duplicate phase transitions.

### B) Missing ownership/authority checks

Any action that mutates shared gameplay state should have a clear owner.

In this project, usually the main player drives sends for their own turn/event. Verify each action has one consistent source of truth, especially in:

- turn transitions
- damage/heal resolution
- draw/discard counts
- victory/lose transitions

### C) Event string mismatch

Because event routing is string-based, tiny naming mismatches silently break behavior.

Example checks:

- Same suffix format (`Player0`, `Player1`)
- Same spelling of phrase names (`Phrase` vs `Phase` in identifiers)
- Same argument keys in `EventArgs`

## 6) Build a minimal "network repro script" per bug

Use `Run/Data/CommandScript.xml` as a seed and keep a tiny script per bug scenario.

For each bug you chase, create a scripted sequence like:

1. Draw fixed number of cards
2. Start one player turn
3. Trigger exactly one problematic action

Goal: reproduce in <= 20 seconds. If you can't do that, debugging time will explode.

## 7) Add lightweight state snapshots around phase transitions

Unexpected bugs often happen at boundaries between phrases. Add a temporary debug dump function in `Player` for:

- current phrase/action state
- hand size
- health
- selected card / manipulate widget context

Print snapshots at entry/exit of each `Event_End*Phrase` and `Event_StartPlayerTurn` callback.

Then compare server/client logs side-by-side; first divergence indicates root-cause location.

## 8) Use binary-search debugging on timeline divergence

When logs are long:

1. Find earliest frame where server/client differ.
2. Check previous transition boundary.
3. Repeat until one callback pair is isolated.

This is faster than inspecting every event linearly.

## 9) Keep temporary debug tooling cheap and removable

Recommended temporary helpers:

- `DebugNetTrace(eventName, args, stage)` helper macro/function
- one-line state dump helper for `Player`
- optional dev-console command to toggle verbose network logs

After fixing, either:

- keep helpers behind a debug flag, or
- remove them in the final cleanup commit.

## 10) A practical "first 30 minutes" workflow

When a new networking bug appears:

1. Repro with server/client bat files.
2. Classify bug type (desync/lost/ordering/authority/UI).
3. Identify single action/event to trace.
4. Add 3-point logs (send, callback enter, callback exit).
5. Compare both process logs and find first divergence.
6. Fix guard/ordering/string mismatch.
7. Re-run same scripted repro 3 times.
8. Only then broaden testing.

---

## Optional next improvement (high ROI)

If you revisit this codebase longer-term, add a shared **event sequence number** in `EventArgs` for outgoing network events. Even a simple incrementing int per sender helps detect drops, duplicates, and ordering issues immediately in logs.
