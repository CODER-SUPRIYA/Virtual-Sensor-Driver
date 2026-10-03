
# Progress Log

## 03 Oct 2026

- Problem: Needed a branching workflow for stage submissions.
  Fix: Created `dev` branch, merged to `main` by PR for each stage.
- Problem: Kernel module can't be built on Windows.
  Fix: Using Ubuntu for driver build, Windows for docs/Git.## 03 Oct 2026 (Stages 3-6)
- Problem: Typos in commands (`linux-header`, `valgrid`, `gitub.com`, `ls/lib`) caused install and clone errors.
  Fix: Read the error text, corrected the spelling, reran.
- Problem: Private repo clone needed authentication.
  Fix: Created a GitHub Personal Access Token and enabled the Git credential helper.
- Problem: Makefile had a Tab on every line (nano auto-indent), so make would fail.
  Fix: Rewrote it with printf so only the recipe lines have a Tab.
- Problem: `make -C driver` failed from the repo root ("No rule to make target Makefile").
  Fix: Makefile used $(PWD), which is the caller's directory. Changed to $(CURDIR).
- Problem: Test script reported a false valgrind failure.
  Fix: `timeout` returns its own exit code, so the test now checks valgrind's output instead.
- Problem: Timer API differs across kernel versions (7.0 here).
  Fix: Used delayed workqueue + miscdevice instead of timer_list and manual cdev setup.
- Problem: Stage tags were first placed on the wrong commit (before merging dev into main).
  Fix: Merged via PR #4 and re-tagged each stage at its real commit.
- Problem: Generated vsensor.mod.c was committed.
  Fix: Removed it from tracking; *.mod.c is in .gitignore.
