require 'xcodeproj'

path_to_project = "#{ENV['CODEPUSH_APP_PATH']}/#{ENV['CODEPUSH_PROJECT_NAME']}.xcodeproj"
puts "CodePush - Path to project: #{path_to_project}"

begin
  project = Xcodeproj::Project.open(path_to_project)
  puts "CodePush - Successfully opened Xcode project."
rescue => e
  puts "CodePush - Error opening Xcode project: #{e.message}"
  exit 1
end

main_target = project.targets.first
puts "CodePush - Main target: #{main_target.name}"

phase_name = "[CodePush] Copy CodePush Bundle"
existing_phase = main_target.shell_script_build_phases.find { |phase| phase.name == phase_name }

if existing_phase
  puts "CodePush - Build phase '#{phase_name}' already exists. Updating it."
  phase = existing_phase
else
  puts "CodePush - Adding new shell script build phase: #{phase_name}"
  phase = main_target.new_shell_script_build_phase(phase_name)
end

# Set the shell script for the build phase
phase.shell_script = <<-SCRIPT
set -e

# Source the .xcode.env file
ENV_PATH="$PODS_ROOT/../.xcode.env"
if [ -f "$ENV_PATH" ]; then
    source "$ENV_PATH"
    echo "Env variables sourced from $ENV_PATH"
else
    echo "Env file $ENV_PATH not found. Ensure it exists."
fi

echo "CodePush - CODEPUSH_COPY_BUNDLE: $CODEPUSH_COPY_BUNDLE"
if [[ "$CODEPUSH_COPY_BUNDLE" == "false" ]]; then
    echo "CodePush - Skipping bundle copy as CODEPUSH_COPY_BUNDLE is set to false."
    exit 0
fi

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
CODEPUSH_BASE_DIR="$APP_ROOT/.codepush"
CODEPUSH_DIR="$CODEPUSH_BASE_DIR/ios"

# Create .codepush and ios directories if they don't exist
echo "CodePush - Creating directory structure at $CODEPUSH_DIR"
mkdir -p "$CODEPUSH_DIR"

echo "CodePush - Copying bundle to CodePush directory"
cp "$BUNDLE_FILE" "$CODEPUSH_DIR/$BUNDLE_NAME.jsbundle"

if [[ -d "$DEST/assets" ]]; then
    echo "CodePush - Copying assets to CodePush directory"
    cp -R "$DEST/assets" "$CODEPUSH_DIR/"
fi
SCRIPT

# Save the project
begin
  project.save
  puts "CodePush - Project saved successfully."
rescue => e
  puts "CodePush - Error saving project: #{e.message}"
  exit 1
end