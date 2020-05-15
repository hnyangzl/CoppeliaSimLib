#pragma once

#include "uiThread.h"
#include "simThread.h"
#include "directoryPaths.h"
#include "userSettings.h"
#include "vMutex.h"
#include "worldContainer.h"
#ifndef SIM_WITHOUT_QT_AT_ALL
    #include "simQApp.h"
    #include "simAndUiThreadSync.h"
#endif
#ifdef SIM_WITH_GUI
    #include "mainWindow.h"
#endif

class App
{
public:
    App(bool headless);
    virtual ~App();

    bool wasInitSuccessful();
    static void setBrowserEnabled(bool e);
    static bool getBrowserEnabled();

    static void beep(int frequ=5000,int duration=1000);
    static void setApplicationName(const char* name);
    static std::string getApplicationName();
    static void createWorldsContainer();
    static void deleteWorldsContainer();

    static void run(void(*initCallBack)(),void(*loopCallBack)(),void(*deinitCallBack)(),bool launchSimThread);
    static void postExitRequest();
    static bool getExitRequest();
    static bool isSimulatorRunning();

    static void simulationThreadInit();
    static void simulationThreadDestroy();
    static void simulationThreadLoop();
    static bool canInitSimThread();

    static void setQuitLevel(int l);
    static int getQuitLevel();

    static std::string getApplicationArgument(int index);
    static void setApplicationArgument(int index,std::string arg);
    static std::string getApplicationNamedParam(const char* paramName);
    static int setApplicationNamedParam(const char* paramName,const char* param,int paramLength);
    static void setAdditionalAddOnScript1(const char* script);
    static std::string getAdditionalAddOnScript1();
    static void setAdditionalAddOnScript2(const char* script);
    static std::string getAdditionalAddOnScript2();

    static bool executeUiThreadCommand(SUIThreadCommand* cmdIn,SUIThreadCommand* cmdOut);
    static void appendSimulationThreadCommand(int cmdId,int intP1=-1,int intP2=-1,float floatP1=0.0,float floatP2=0.0,const char* stringP1=nullptr,const char* stringP2=nullptr,int executionDelay=0);
    static void appendSimulationThreadCommand(SSimulationThreadCommand cmd,int executionDelay=0);

    static int getEditModeType(); // helper
    static void setRebuildHierarchyFlag(); // helper
    static void setResetHierarchyViewFlag(); // helper
    static void setRefreshHierarchyViewFlag(); // helper
    static void setLightDialogRefreshFlag(); // helper
    static void setFullDialogRefreshFlag(); // helper
    static void setDialogRefreshDontPublishFlag(); // helper
    static void setToolbarRefreshFlag(); // helper
    static int getMouseMode(); // helper
    static void setMouseMode(int mm); // helper
    static void setDefaultMouseMode(); // helper
    static bool isFullScreen(); // helper
    static void setFullScreen(bool f); // helper

    static bool logPluginMsg(const char* pluginName,int verbosityLevel,const char* logMsg);
    static void logMsg(int verbosityLevel,const char* msg,const char* subStr1=nullptr,const char* subStr2=nullptr,const char* subStr3=nullptr);
    static void logMsg(int verbosityLevel,const char* msg,int int1,int int2=0,int int3=0);
    static int getConsoleVerbosity(const char* pluginName=nullptr);
    static void setConsoleVerbosity(int v,const char* pluginName=nullptr);
    static int getStatusbarVerbosity(const char* pluginName=nullptr);
    static void setStatusbarVerbosity(int v,const char* pluginName=nullptr);
    static bool getConsoleOrStatusbarVerbosityTriggered(int verbosityLevel);
    static int getVerbosityLevelFromString(const char* verbosityStr);
    static bool getConsoleMsgToFile();
    static void setConsoleMsgToFile(bool f);

    static void addStatusbarMessage(const std::string& txt,bool scriptErrorMsg=false,bool notToConsole=false);
    static void clearStatusbar();

    static float* getRGBPointerFromItem(int objType,int objID1,int objID2,int colComponent,std::string* auxDlgTitle);
    static CColorObject* getVisualParamPointerFromItem(int objType,int objID1,int objID2,std::string* auxDlgTitle,int* allowedParts);
    static CTextureProperty* getTexturePropertyPointerFromItem(int objType,int objID1,int objID2,std::string* auxDlgTitle,bool* is3D,bool* valid,CGeometric** geom);

    static CDirectoryPaths* directories;
    static CUserSettings* userSettings;
    static CWorldContainer* worldContainer;
    static CWorld* currentWorld; // actually worldContainer->currentWorld
    static CUiThread* uiThread;
    static CSimThread* simThread;

    static int operationalUIParts;
    static int sc;

private:
    static void _logMsg_noDecoration(int verbosityLevel,const char* msg,const char* subStr1=nullptr,const char* subStr2=nullptr,const char* subStr3=nullptr);
    static void _logMsg_noDecoration(int verbosityLevel,const char* msg,int int1,int int2=0,int int3=0);
    static void _logMsg(int verbosityLevel,const char* msg,bool forbidStatusbar,int consoleVerbosity=-1,int statusbarVerbosity=-1);
    static std::string _getDecoratedLogMsg(const char* pluginName,int verbosityLevel,const char* msg);
    bool _initSuccessful;
    static bool _consoleMsgsToFile;
    static VFile* _consoleMsgsFile;
    static VArchive* _consoleMsgsArchive;

    static bool _browserEnabled;
    static bool _canInitSimThread;

    static void _runInitializationCallback(void(*initCallBack)());
    static void _runDeinitializationCallback(void(*deinitCallBack)());
    static void _processGuiEventsUntilQuit();

//  static VTHREAD_ID_TYPE _guiThread;
    static bool _exitRequest;
    static bool _simulatorIsRunning;
    static std::string _applicationName;
    static std::vector<std::string> _applicationArguments;
    static std::map<std::string,std::string> _applicationNamedParams;
    static std::string _additionalAddOnScript1;
    static std::string _additionalAddOnScript2;
    static int _consoleVerbosity;
    static int _statusbarVerbosity;

    static volatile int _quitLevel;

#ifndef SIM_WITHOUT_QT_AT_ALL
public:
    static CSimQApp* qtApp;

private:
    static int _qApp_argc;
    static char _qApp_arg0[];
    static char* _qApp_argv[1];
#endif

#ifdef SIM_WITH_GUI
public:
    static void showSplashScreen();
    static void setIcon();
    static CMainWindow* mainWindow;
    static void createMainWindow();
    static void deleteMainWindow();
    static void setShowConsole(bool s);
#endif
};

class CFuncTrace
{
public:
    CFuncTrace(const char* functionName,int traceVerbosity)
    {
        if (App::getConsoleOrStatusbarVerbosityTriggered(traceVerbosity))
        {
            _txt=functionName;
            _verbosity=traceVerbosity;
            if (_verbosity==sim_verbosity_traceall)
                _txt+=" (C)";
            if (_verbosity==sim_verbosity_tracelua)
                _txt+=" (Lua API)";
            App::logMsg(_verbosity,(std::string("--> ")+_txt).c_str());
        }
    };
    virtual ~CFuncTrace()
    {
        if (_txt.size()>0)
            App::logMsg(_verbosity,(std::string("<-- ")+_txt).c_str());
    };

private:
    std::string _txt;
    int _verbosity;
};
