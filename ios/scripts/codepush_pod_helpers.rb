def codepush_post_install(installer, project_name, app_path, node_modules_path = '../node_modules')
    command = 'ruby'
    args = ["#{node_modules_path}/@itspar/codepush-sdk/ios/scripts/modify_xcodeproj.rb"]
    full_command = [command, *args].join(' ')
    env_vars = {
      'CODEPUSH_APP_PATH' => app_path,
      'CODEPUSH_PROJECT_NAME' => project_name
    }
  
    Pod::UI.puts "Executing CodePush script to add Copy Bundle in Build Phase: #{full_command}"
  
    success = system(env_vars, command, *args)
    unless success
      raise "CodePush script failed: #{full_command}"
    end
  end