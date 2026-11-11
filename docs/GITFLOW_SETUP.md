# Set up Git Flow in an EXISTING GitHub repository

**Important**: This kit provides configuration files only. It does not initialize Git, change your remotes, delete branches, or rewrite commits. Back up the repo or work from a clean commit before applying.

## 1. Confirm state

```bash
git status
git remote -v
git branch -a
git fetch origin --prune
```

If there are uncommitted changes, commit or safely stash them before switching branches. The examples assume your primary branch is `main`; if it is `master`, decide whether to rename it, or adapt commands accordingly. Do not blindly run `git push --force`.

## 2. Establish develop once

```bash
git switch main
git pull --ff-only origin main
git switch -c develop
git push -u origin develop
```

If `develop` already exists, DO NOT run `git switch -c develop`; instead use `git switch develop` (and `git pull --ff-only origin develop` when tracking is configured), or use `git switch --track origin/develop` if only remote exists.

## 3. Add kit via feature PR

```bash
git switch develop
git pull --ff-only origin develop
git switch -c feature/1-gitflow-setup
# Copy this kit's .github/, docs/ and CONTRIBUTING.md into the repo.
# Inspect differences before staging:
git diff --check
git status
git add .github/ CONTRIBUTING.md docs/GITFLOW_SETUP.md docs/PROJECT_BOARD.md
git commit -m 'chore(repo): add Git Flow and CI (#1)'
git push -u origin feature/1-gitflow-setup
```

Create a PR on GitHub: `feature/1-gitflow-setup` → `develop`. The `(#1)` reference is an example; use the real issue ID.

## 4. Configure GitHub rulesets / branch protection

Repository Settings → Rules → Rulesets (or Settings → Branches, depending on repository UI and available permissions):

For both `main` and `develop`:
- Require PR before merging; forbid direct pushes (no bypass except necessary maintainers).
- Require status checks after the first CI run: `build-test (gcc)` and `build-test (clang)`.
- Block force pushes and branch deletion.
- Require conversation resolution; optional 1 independent approval when working with teammates.

`main` release PRs should use a merge commit. Feature PRs to `develop` should normally be squashed. The PR UI shows the current base and merge strategy — check it before clicking merge.

If branch rules are unavailable for the repository visibility/plan, follow the policy manually and keep CI enabled.

## 5. Workflow for the first feature

```bash
git fetch origin
git switch develop
git pull --ff-only origin develop
git switch -c feature/2-sentence-parser
# Edit code and add tests
make clean && make && make test
git add src/core/sentence_parser.c tests/test_sentence_parser.c
git commit -m 'feat(parser): detect sentence completion (#2)'
git push -u origin feature/2-sentence-parser
```

Then open PR to develop. Use actual issue number, and check CI results before merge.

## 6. Troubleshooting

- `fatal: a branch named develop already exists` → reuse the existing branch, do not delete it.
- `fatal: refusing to merge unrelated histories` → STOP; verify that local and remote repositories are the same project. Never add `--allow-unrelated-histories` without understanding both histories.
- CI cannot find Makefile → align the workflow's working directory with your repo; this kit assumes Makefile is at the repo root.
- CI cannot pass due to GTK4 or libcurl later → install the necessary `-dev` packages in CI and update the Makefile linker flags in a dedicated PR.
- Can't self-approve a PR → solo contributors can rely on CI and self-review if branch policy permits; independent review requires a second person.
