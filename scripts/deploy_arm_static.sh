#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
PROJECT_DIR=$(cd "$SCRIPT_DIR/.." && pwd)
CONFIG_FILE="${1:-$SCRIPT_DIR/deploy.env}"

if [[ ! -f "$CONFIG_FILE" ]]; then
    echo "Missing config: $CONFIG_FILE" >&2
    echo "Copy scripts/deploy.example.env to scripts/deploy.env and edit it." >&2
    exit 1
fi

# shellcheck source=/dev/null
source "$CONFIG_FILE"

: "${BOARD_HOST:?BOARD_HOST is required}"
: "${BOARD_USER:?BOARD_USER is required}"
: "${BOARD_DIR:?BOARD_DIR is required}"

if [[ "$BOARD_HOST" == "your-board-ip" ]]; then
    echo "BOARD_HOST still uses the example placeholder. Edit $CONFIG_FILE first." >&2
    exit 1
fi

if [[ "$BOARD_DIR" == "/path/on/board" ]]; then
    echo "BOARD_DIR still uses the example placeholder. Edit $CONFIG_FILE first." >&2
    exit 1
fi

ARM_CC="${ARM_CC:-arm-linux-gnueabihf-gcc}"
LOCAL_BINARY="${LOCAL_BINARY:-build/lumina_frame_arm}"
REMOTE_BINARY="${REMOTE_BINARY:-lumina_frame}"
SSH_CONNECT_TIMEOUT="${SSH_CONNECT_TIMEOUT:-8}"
DEPLOY_RETRIES="${DEPLOY_RETRIES:-3}"
DEPLOY_RETRY_DELAY="${DEPLOY_RETRY_DELAY:-2}"
SSH_STRICT_HOST_KEY_CHECKING="${SSH_STRICT_HOST_KEY_CHECKING:-accept-new}"
SSH_TARGET="$BOARD_USER@$BOARD_HOST"

run_with_retry() {
    local desc="$1"
    shift
    local attempt=1

    while true; do
        if "$@"; then
            return 0
        fi

        if [[ "$attempt" -ge "$DEPLOY_RETRIES" ]]; then
            echo "$desc failed after $attempt attempt(s)." >&2
            return 1
        fi

        echo "$desc failed on attempt $attempt, retrying in ${DEPLOY_RETRY_DELAY}s..." >&2
        sleep "$DEPLOY_RETRY_DELAY"
        attempt=$((attempt + 1))
    done
}

cd "$PROJECT_DIR"

make arm-static ARM_CC="$ARM_CC"

run_with_retry "Create remote directory"     ssh -o ConnectTimeout="$SSH_CONNECT_TIMEOUT"     -o StrictHostKeyChecking="$SSH_STRICT_HOST_KEY_CHECKING"     "$SSH_TARGET" "mkdir -p '$BOARD_DIR'"

run_with_retry "Copy binary"     scp -o ConnectTimeout="$SSH_CONNECT_TIMEOUT"     -o StrictHostKeyChecking="$SSH_STRICT_HOST_KEY_CHECKING"     "$LOCAL_BINARY" "$SSH_TARGET:$BOARD_DIR/$REMOTE_BINARY"

echo "Deployed $LOCAL_BINARY to $SSH_TARGET:$BOARD_DIR/$REMOTE_BINARY"
