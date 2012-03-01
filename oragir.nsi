;NSIS Oragir Install Script
;by Vahan Aghajanyan

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "WordFunc.nsh"

;--------------------------------
;General

  ; Define version
  !define VERSION "0.2"
  !define NAME "Oragir"

  ;Name and file
  Name "${NAME}"
  OutFile "installation\Oragir-Win32-v${VERSION}.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES32\${NAME}"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\${NAME}" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
; Functions
Function .onInit

  ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" \
  "UninstallString"
  StrCmp $R0 "" done

  ; Get the installed version.
  ReadRegStr $R1 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" \
  "DisplayVersion"

  ; Convert version if some characters used, f. e. "0.2.226b"
  ${VersionConvert} "$R1" "" $R1

  ; Convert the current version too.
  ${VersionConvert} "${VERSION}" "" $R2

  ; Compare new and installed versions.
  ${VersionCompare} "$R2" "$R1" "$R3"

  ; Decide what to do.
  IntCmp $R3 1 ask_to_update same_version do_not_update

do_not_update:
  MessageBox MB_OK|MB_ICONEXCLAMATION \
  "The more recent version $R1 of ${NAME} is already installed."
  Abort

same_version:
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "${NAME} $R1 is already installed. $\n$\nClick `OK` to reinstall existing \
  version or `Cancel` to cancel this upgrade." \
  IDOK done
  Abort

ask_to_update:
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "${NAME} is already installed. $\n$\nClick `OK` to remove the \
  previous version $R1 and install new version ${VERSION} or `Cancel` \
  to cancel this upgrade." \
  IDOK uninst
  Abort

;Run the uninstaller
uninst:
  ClearErrors
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file

  IfErrors no_remove_uninstaller done
    ;You can either use Delete /REBOOTOK in the uninstaller or add some code
    ;here to remove the uninstaller. Use a registry key to check
    ;whether the user has chosen to uninstall. If you are using an uninstaller
    ;components page, make sure all sections are uninstalled.
  no_remove_uninstaller:

done:

FunctionEnd

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "distrib\License.txt"

  ; Disabled components page
  ;!insertmacro MUI_PAGE_COMPONENTS

  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${NAME}"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Oragir" SecDummy

  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...
  File "release\oragir.exe"
  File "release\ljcommunicator.dll"
  File "distrib\msvcr90.dll"
  File "distrib\msvcp90.dll"
  File "distrib\Microsoft.VC90.CRT.manifest"
  File "distrib\QtCore4.dll"
  File "distrib\QtGui4.dll"
  File "distrib\QtNetwork4.dll"
  File "distrib\QtSql4.dll"
  File "distrib\QtWebKit4.dll"
  File "distrib\QtXml4.dll"
  File "distrib\license.txt"
  File ".\oragir\qt.conf"
  SetOutPath "$INSTDIR\sqldrivers"
  File "distrib\qsqlite4.dll"
  SetOutPath "$INSTDIR\imageformats"
  File "distrib\qgif4.dll"
  ;SetOutPath "$INSTDIR\translations"
  ;File "oragir_ru.qm"

  ;Store installation folder
  WriteRegStr HKCU "Software\${NAME}" "" $INSTDIR
  WriteRegStr HKCU "Software\${NAME}" "Version" ${VERSION}

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	CreateShortCut "$SMPROGRAMS\$StartMenuFolder\${NAME}.lnk" "$INSTDIR\oragir.exe"

  !insertmacro MUI_STARTMENU_WRITE_END

  ; Add uninstall information to Add/Remove Programs
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayName" "${NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "Publisher" "Vahan Aghajanyan"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayVersion" "${VERSION}"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  ;LangString DESC_SecDummy ${LANG_ENGLISH} "${NAME} binary and configuration files."

  ;Assign language strings to sections
  ;!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	;!insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  ;!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\oragir.exe"
  Delete "$INSTDIR\ljcommunicator.dll"
  Delete "$INSTDIR\msvcr90.dll"
  Delete "$INSTDIR\msvcp90.dll"
  Delete "$INSTDIR\Microsoft.VC90.CRT.manifest"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtWebKit4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\qt.conf"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\sqldrivers\qsqlite4.dll"
  Delete "$INSTDIR\imageformats\qgif4.dll"
  RMDir "$INSTDIR\sqldrivers"
  RMDir "$INSTDIR\imageformats"
  ;Delete "$INSTDIR\translations\oragir_ru.qm"
  ;RMDir "$INSTDIR\translations"
  RMDir "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\${NAME}.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\${NAME}"

  ; Remove uninstall information to Add/Remove Programs
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}"

SectionEnd
