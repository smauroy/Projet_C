# AutoTranslate Git Flow kit

A non-destructive template for adding Git Flow documentation and GitHub CI/Issue/PR templates to an EXISTING repository.

## Files

- `.github/workflows/ci.yml`: gcc and clang build/test on PRs and branch pushes.
- `.github/PULL_REQUEST_TEMPLATE.md`: PR quality checklist.
- `.github/ISSUE_TEMPLATE/*.yml`: feature, bug, engineering task forms.
- `CONTRIBUTING.md`: naming, commits, reviews, release and hotfix process.
- `docs/PROJECT_BOARD.md`: first sprint and task board.
- `docs/GITFLOW_SETUP.md`: careful setup commands for existing repos.

## Usage

Extract ZIP outside your repository. From your own checked-out feature branch, copy the desired files into the repo without overwriting existing templates/configuration; inspect with `git diff` and `git status` before committing. Then follow `docs/GITFLOW_SETUP.md`.

The CI assumes a root-level `Makefile` with `make`, `make test`, and `make clean`; it is designed for the provided AutoTranslate Day 1 starter and should be adjusted for a different layout.
