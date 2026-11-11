# AutoTranslate — GitHub Project board

## Suggested views / columns

Backlog → Ready → In progress → In review → Done

Create a GitHub Project and fields: Status, Priority (P0/P1/P2/P3), Sprint (optional), Owner, Target version (v0.1.0...). Add Issues to this project.

## First sprint / MVP issues

| Issue title | Acceptance criteria | Suggested branch |
|---|---|---|
| `chore: repository Git Flow setup` | Rules and CI configured, bootstrap builds | `feature/1-gitflow-setup` |
| `feat: sentence completion parser` | Detect .!? with basic UTF-8 tests; avoid double triggers | `feature/2-sentence-parser` |
| `feat: bilingual document model` | Preserve source text + translation pair | `feature/3-document-model` |
| `feat: GTK4 editor bootstrap` | Open window, enter text, display translation area | `feature/4-gtk-editor` |
| `feat: HTTP translation adapter` | Request + parse response, timeouts and error paths tested | `feature/5-translation-api` |
| `feat: async translation pipeline` | Network does not freeze GTK UI | `feature/6-async-translation` |
| `feat: local persistence` | Save/load UTF-8 source and translated text | `feature/7-file-storage` |
| `test: end-to-end MVP and release` | Linux demo, build/test all green, README | `feature/8-mvp-tests` |

Replace the example issue numbers with the actual IDs assigned by GitHub.
