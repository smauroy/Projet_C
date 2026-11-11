# Contributing to AutoTranslate

This project uses Git Flow as a learning and collaboration workflow.

## Branch policy

| Branch | Create from | Merge into | Purpose |
|---|---|---|---|
| `main` | existing primary branch | — | Stable releases only |
| `develop` | `main` once | — | Integration |
| `feature/<issue>-<slug>` | `develop` | `develop` | Features and engineering tasks |
| `release/vX.Y.Z` | `develop` | `main`, then sync to `develop` | Release stabilization |
| `hotfix/vX.Y.Z` | `main` | `main`, then sync to `develop` | Urgent production fix |

Do not force-push shared branches. Keep feature branches short-lived.

## Everyday workflow

1. Open a GitHub Issue with acceptance criteria.
2. `git fetch origin` and start a new feature branch from the latest `origin/develop`.
3. Write and test code locally (`make && make test`).
4. Use small Conventional Commits:
   - `feat(parser): detect sentence ending (#2)`
   - `fix(storage): handle empty file (#17)`
   - `test(parser): add UTF-8 edge cases (#2)`
   - `docs: document branch policy (#1)`
   - `chore(ci): enable gcc and clang (#1)`
5. Push the feature branch and open a PR to `develop`, link the Issue.
6. Await CI and review, then **Squash and merge** the feature PR.
7. Delete the feature branch after merge.

Reviewers should check correct behavior, error handling, input validation, pointer ownership, and tests. Do not blindly accept generated code.

## Release workflow

1. From updated `develop`, create `release/v0.1.0`.
2. Only fix blocking bugs, docs, and version metadata there.
3. Open PR `release/v0.1.0` → `main` and **create a merge commit**, not squash.
4. After main is merged, create an annotated tag `v0.1.0` on the merged `main` commit and push the tag.
5. Open PR `main` → `develop` (merge commit) to bring release fixes back. If extra work landed in `develop`, resolve carefully and keep both histories; never force-push.

## Hotfix workflow

1. From updated `main`, create `hotfix/v0.1.1`.
2. Fix the bug; add a regression test; open PR to `main`.
3. Merge with a merge commit; tag main with `v0.1.1`.
4. Open PR `main` → `develop` to include the hotfix, then delete hotfix branch.

## Definition of done

- Issue acceptance criteria are met.
- CI passes for gcc and clang.
- Tests cover key paths and errors.
- No credentials or secrets in source control.
- PR reviewed; documentation updated if needed.

GitHub permission limits may affect ruleset availability and self-approval; solo work can use CI and documented self-review when independent approval is not possible.
