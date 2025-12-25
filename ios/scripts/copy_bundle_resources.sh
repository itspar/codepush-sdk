#!/bin/bash
set -e

BUNDLE_NAME="main"
DEST="$CONFIGURATION_BUILD_DIR/$UNLOCALIZED_RESOURCES_FOLDER_PATH"
BUNDLE_FILE="$DEST/$BUNDLE_NAME.jsbundle"

echo "CodePush - BUNDLE_FILE: $BUNDLE_FILE"
echo "CodePush - CONFIGURATION: $CONFIGURATION"

if [[ "$CONFIGURATION" != "Release" ]]; then
    echo "Skipping CodePush bundle copy for ${CONFIGURATION} build"
    exit 0
fi


if [[ ! -f "$BUNDLE_FILE" ]]; then
    echo "Warning: Bundle not found at $BUNDLE_FILE"
    exit 0
fi

APP_ROOT="$SRCROOT/.."
CODEPUSH_DIR="$APP_ROOT/.codepush"
CODEPUSH_DIR="$CODEPUSH_DIR/ios"

# Create .codepush and ios directories if they don't exist
echo "CodePush - Creating directory structure at $CODEPUSH_DIR"
mkdir -p "$CODEPUSH_DIR"

echo "CodePush - Copying bundle to CodePush directory"
cp "$BUNDLE_FILE" "$CODEPUSH_DIR/$BUNDLE_NAME.jsbundle"

if [[ -d "$DEST/assets" ]]; then
    echo "CodePush - Copying assets to CodePush directory"
    cp -R "$DEST/assets" "$CODEPUSH_DIR/"
fi