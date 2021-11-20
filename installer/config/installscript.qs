function Component()
{
	component.loaded.connect(this, addOptionsCheckBoxForm);
}

addOptionsCheckBoxForm = function()
{
    // don't show when updating or uninstalling
    if (installer.isInstaller()) {
        installer.addWizardPageItem(component, "OptionsCheckBoxForm", QInstaller.TargetDirectory);
        var form = component.userInterface("OptionsCheckBoxForm");

      var startMenuCheckbox = form.CreateStartMenuEntry;
      startMenuCheckbox.stateChanged.connect(this, function() {
      installer.setDefaultPageVisible(QInstaller.StartMenuSelection, startMenuCheckbox.checked);
      });
    }
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    
    var form = component.userInterface("OptionsCheckBoxForm");
    var isStartMenuEntryChecked = form.CreateStartMenuEntry.checked;
    var forAllUsersChecked = form.CreateStartMenuEntry.ForAllUsers.checked;
    var executable = "";
    
    if (installer.value("os") === "win") {
    	executable = "@TargetDir@/bin/webpassware.exe";
    }
    else {
        executable = "@TargetDir@/bin/webpassware";
    }
    
    var linkPrefix = "@UserStartMenuProgramsPath@";
		if (forAllUsersChecked) {
			linkPrefix = "@AllUsersStartMenuProgramsPath@";
		}

	if (isStartMenuEntryChecked) {
		if (installer.value("os") === "win") {
			component.addOperation("CreateShortcut", 
			                       executable,
			                       linkPrefix + "/@StartMenuDir@/WebPassWare.lnk",
				                   "workingDirectory=@TargetDir@",
				                   "iconPath=@TargetDir@/webpassware.ico",
				                   "iconId=0",
				                   "description=WebPassWare");
		}

		component.addOperation( "CreateDesktopEntry",
                            "WebPassWare.desktop",
                            "Comment=WebPassWare do przechowywania haseł do serwisów www / poczty itp\n" +
                            "Type=Application\n" +
                            "Exec=" + executable + "\n" +
                            "Path=@TargetDir@\n" +
                            "Name=WebPassWare\n" +
                            "Icon=@TargetDir@/webpassware.png\n" +
                            "Terminal=false\n" +                            
                            "MimeType=");
     }
}
