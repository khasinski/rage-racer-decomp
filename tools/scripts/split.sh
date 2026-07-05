#!/usr/bin/env bash
# Run splat on the configured Rage Racer executable.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"
PY="${ROOT}/.venv/bin/python"
if [[ ! -x "$PY" ]]; then
    PY=python3
fi
VERSION="${VERSION:-PAL}"
exec "$PY" -m splat split "configs/${VERSION}/main.yaml" "$@"
