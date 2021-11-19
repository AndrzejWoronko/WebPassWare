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
    	executable = "@TargetDir@/MultiFirma.exe";
    }
    else {
        executable = "@TargetDir@/multifirma"; 
    }
    
    var linkPrefix = "@UserStartMenuProgramsPath@";
		if (forAllUsersChecked) {
			linkPrefix = "@AllUsersStartMenuProgramsPath@";
		}

	if (isStartMenuEntryChecked) {
		if (installer.value("os") === "win") {
			component.addOperation("CreateShortcut", 
			                       executable,
			                       linkPrefix + "/@StartMenuDir@/Multifirma.lnk",
				                   "workingDirectory=@TargetDir@",
				                   "iconPath=@TargetDir@/multifirma.ico",
				                   "iconId=0",
				                   "description=MultiFirma");
		}

		component.addOperation( "CreateDesktopEntry",
                            "Multifirma.desktop",
                            "Comment=MultiFirma program do prowadzenia działalności gospodarczej\n" +
                            "Type=Application\n" +
                            "Exec=" + executable + "\n" +
                            "Path=@TargetDir@\n" +
                            "Name=MultiFirma\n" +
                            "Icon=@TargetDir@/multifirma.png\n" +
                            "Terminal=false\n" +                            
                            "MimeType=");
     }
}
