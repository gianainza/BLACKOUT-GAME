# BLACKOUT - R5 Sprint 1 (STRICT ADD-ONLY)

## What is in this package
- `QA/FrameTimeLogger.h` - **new**, R5-owned C++14 timing harness.
- `QA/THREE_MACHINE_BASELINE_TEMPLATE.csv` - **new**, blank machine comparison table.
- `QA/R5_INSTRUCTIONS.md` - **new**, documentation.

## Preservation guarantee
The full-project ZIP is made from the original `BLACKOUT-GAME-main.zip` you uploaded.
**Every original file is included with identical file bytes.** No source file, shader,
scene, texture, Visual Studio project, or existing compiled artifact was edited or removed.
Your Git diff after copying ONLY the QA directory should show **new files only**.

### Important: the harness is not yet active
A C++ header cannot time the application's main loop until `main.cpp` calls it.
`main.cpp` is R1-owned, so **no calls were inserted in this strict add-only version**.
An OPTIONAL `R5_main_ADD_LINES_ONLY_for_R1.patch` is provided separately for R1 to review.
It inserts R5-only lines into `main.cpp`, **without removing or altering any existing line**.
Applying that patch WILL make `main.cpp` appear modified in Git, even though it is
additions-only. Have R1 approve it and commit it under the agreed ownership rules.

## Integrating (R1-approved option)
From the repository root (the directory containing `.gitignore`), run:

```powershell
git apply --check "R5_main_ADD_LINES_ONLY_for_R1.patch"
git apply "R5_main_ADD_LINES_ONLY_for_R1.patch"
```

This assumes you put the separate patch at the repository root first. If the first
command fails, **do not force it**. R1 may already have edited `main.cpp`.
Ask R1 to integrate manually based on the patch.

## Running after integration
1. Build the project with R1's dependencies resolved. Prefer Release x64.
2. For Visual Studio: Project Properties -> Debugging -> Environment, add:
   `BLACKOUT_R5_BENCHMARK=1` (set the project working directory to `$(ProjectDir)`).
   For PowerShell launching a built EXE, first run:

   ```powershell
   $env:BLACKOUT_R5_BENCHMARK = "1"
   ```

   Then launch the actual game EXE in that session.
3. The game warms up for 120 frames and records the following 300 frames.
   It prints mean, median, P95, minimum and maximum frame time in ms and
   saves `R5_FrameTimes.csv` in the process working directory.
4. The benchmark's VSync is disabled, and normal runs (without the environment
   variable) behave as before.
5. Copy/rename the CSV after each run; new runs overwrite the same filename.

## What is measured
The logger measures CPU-observed full frame-loop time including buffer swap,
not pure GPU time. It does not invent any timings. Run on three **different**
machines and record each machine's GPU, CPU, RAM, OS, scene, project commit,
build configuration, window size, and actual average time.

**Unlit warning:** The uploaded `main.cpp` still runs lighting/shadow passes.
Do not label that output an unlit baseline. Coordinate a separate unlit
configuration with R1/R2 first, using the same scene/settings for fair comparison.

## Git: safe R5-only commit
After you copy the new `QA` files into your EXISTING repository:

```powershell
git status --short
git add "Skybox Activity/ActivityJ_Skybox/SkyboxLab/QA"
git diff --cached --name-status
git commit -m "test(qa): add R5 frame-time benchmarking harness and templates"
```

Expected status is `A` for the new QA files, and **no `M` or `D`**.
Do NOT stage or commit `main.cpp` as R5 without R1 approval.

## Academic disclosure
The project brief requires declaration of external or AI-generated code in the
group decision log with file names and line ranges. Be able to explain the
clock, warmup, 300 samples, CSV, and limitations yourself.
