#include "simInternal.h"
#include "dlgCont.h"
#include "global.h"
#include "oglSurface.h"
#include "app.h"
#include "auxLibVideo.h"
#include "simStrings.h"
#include "qdlgobjectdialogcontainer.h"
#include "qdlgshapeeditioncontainer.h"
#include "sceneObjectOperations.h"
#include "qdlgopenglsettings.h"
#include "libLic.h"

CDlgCont::CDlgCont(QWidget* pWindow)
{
    _destroyingContainerNow=false;
    initialize(pWindow);
}

CDlgCont::~CDlgCont()
{
    killAllDialogs();
}

void CDlgCont::initialize(QWidget* pWindow)
{
    if (dialogs.size()!=0)
        return; // Already initialized!
    parentWindow=pWindow;

    dialogs.push_back(new CToolDlgWrapper(DETECTION_VOLUME_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(SHAPE_DYN_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(SHAPE_EDITION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(OBJECT_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(CALCULATION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(JOINT_DYN_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(TRANSLATION_ROTATION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(PATH_EDITION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(MULTISHAPE_EDITION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(GENERAL_PROPERTIES_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(SHAPE_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(JOINT_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(CAMERA_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(LIGHT_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(MIRROR_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(PROXIMITY_SENSOR_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(VISION_SENSOR_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(MILL_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(FORCE_SENSOR_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(GRAPH_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(SETTINGS_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(SELECTION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(COLLECTION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(ENVIRONMENT_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(COLLISION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(DISTANCE_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(IK_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(LUA_SCRIPT_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(DUMMY_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(LAYERS_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(DYNAMICS_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(PATH_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(SIMULATION_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(FOG_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(MATERIAL_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(LIGHTMATERIAL_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(COLOR_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(TEXTURE_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(GEOMETRY_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(GRAPH2DAND3DCURVES_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(IKELEMENT_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(PATH_SHAPING_DLG,0));
    dialogs.push_back(new CToolDlgWrapper(AVI_RECORDER_DLG,0));
}

void CDlgCont::refresh()
{
    if (!_destroyingContainerNow)
    {
        for (size_t i=0;i<dialogs.size();i++)
            dialogs[i]->refresh();
    }
}

void CDlgCont::callDialogFunction(const SUIThreadCommand* cmdIn,SUIThreadCommand* cmdOut)
{
    if ( (!_destroyingContainerNow)&&(cmdIn!=nullptr) )
    {
        for (size_t i=0;i<dialogs.size();i++)
            dialogs[i]->dialogCallbackFunc(cmdIn,cmdOut);
    }
}

void CDlgCont::destroyWhatNeedsDestruction()
{
    if (!_destroyingContainerNow)
    {
        for (size_t i=0;i<dialogs.size();i++)
            dialogs[i]->destroyIfDestructionNeeded();
    }
}

void CDlgCont::visibleInstanceAboutToSwitch()
{
    if (!_destroyingContainerNow)
    {
        for (size_t i=0;i<dialogs.size();i++)
            dialogs[i]->visibleInstanceAboutToSwitch();
    }
}

void CDlgCont::showDialogsButDontOpenThem()
{
    if (!_destroyingContainerNow)
    {
        for (size_t i=0;i<dialogs.size();i++)
            dialogs[i]->showDialogButDontOpenIt();
    }
}

void CDlgCont::hideDialogsButDontCloseThem()
{
    if (!_destroyingContainerNow)
    {
        for (size_t i=0;i<dialogs.size();i++)
            dialogs[i]->hideDialogButDontCloseIt();
    }
}

bool CDlgCont::openOrBringToFront(int dlgID)
{
    if (!_destroyingContainerNow)
    {
        if (dlgID==HIERARCHY_DLG)
            App::mainWindow->oglSurface->setHierarchyEnabled(true);
        else if (dlgID==BROWSER_DLG)
            App::setBrowserEnabled(true);
        else
        {   // Here we check if we can open the dialog: (check also "toggle" function!)
            if ((App::operationalUIParts&sim_gui_dialogs)==0)
                return(false);
            CToolDlgWrapper* it=_getDialogWrapper(dlgID);
            if (it!=nullptr)
                it->setVisible(true,parentWindow);
        }
        App::setToolbarRefreshFlag();
        return(true);
    }
    return(false);
}

void CDlgCont::close(int dlgID)
{
    if (!_destroyingContainerNow)
    {
        if (dlgID==HIERARCHY_DLG)
            App::mainWindow->oglSurface->setHierarchyEnabled(false);
        else if (dlgID==BROWSER_DLG)
            App::setBrowserEnabled(false);
        else
        {
            CToolDlgWrapper* it=_getDialogWrapper(dlgID);
            if (it!=nullptr)
                it->setVisible(false,parentWindow);
        }
        App::setToolbarRefreshFlag();
    }
}

bool CDlgCont::toggle(int dlgID)
{
    if (!_destroyingContainerNow)
    {
        if (dlgID==HIERARCHY_DLG)
            App::mainWindow->oglSurface->setHierarchyEnabled(!App::mainWindow->oglSurface->isHierarchyEnabled());
        else if (dlgID==BROWSER_DLG)
            App::setBrowserEnabled(!App::getBrowserEnabled());
        else
        {
        // Here we check if we can open the dialog: (check also "openOrBringToFront" function!)
            if ((App::operationalUIParts&sim_gui_dialogs)==0)
                return(false);
            CToolDlgWrapper* it=_getDialogWrapper(dlgID);
            if (it!=nullptr)
                it->setVisible(!it->getVisible(),parentWindow);
        }
        App::setToolbarRefreshFlag();
        return(true);
    }
    return(false);
}

CToolDlgWrapper* CDlgCont::_getDialogWrapper(int dlgID)
{
    if (!_destroyingContainerNow)
    {
        for (size_t i=0;i<dialogs.size();i++)
        {
            if (dialogs[i]->getDialogType()==dlgID)
                return(dialogs[i]);
        }
    }
    return(nullptr);
}

VDialog* CDlgCont::getDialog(int dlgID)
{
    if (!_destroyingContainerNow)
    {
        for (int i=0;i<int(dialogs.size());i++)
        {
            if (dialogs[i]->getDialogType()==dlgID)
                return(dialogs[i]->getDialog());
        }
    }
    return(nullptr);
}

bool CDlgCont::isVisible(int dlgID)
{
    if (!_destroyingContainerNow)
    {
        if (dlgID==HIERARCHY_DLG)
            return(App::mainWindow->oglSurface->isHierarchyEnabled());
        else if (dlgID==BROWSER_DLG)
            return(App::getBrowserEnabled());
        else
        {
            CToolDlgWrapper* it=_getDialogWrapper(dlgID);
            if (it!=nullptr)
                return(it->getVisible());
        }
    }
    return(false);
}

void CDlgCont::killAllDialogs()
{
    _destroyingContainerNow=true;
    for (int i=0;i<int(dialogs.size());i++)
        delete dialogs[i];
    dialogs.clear();
}

void CDlgCont::getWindowPos(int dlgID,int pos[2],bool& visible)
{
    if (!_destroyingContainerNow)
    {
        if (dlgID==HIERARCHY_DLG)
        {
            pos[0]=App::mainWindow->oglSurface->getHierarchyWidth();
            pos[1]=0;
            visible=App::mainWindow->oglSurface->isHierarchyEnabled();
        }
        else if (dlgID==BROWSER_DLG)
        {
        }
        else
        {
            CToolDlgWrapper* it=_getDialogWrapper(dlgID);
            if (it!=nullptr)
            {
                it->getPosition(pos);
                visible=it->getVisible();
            }
        }
    }
}

void CDlgCont::setWindowPos(int dlgID,int pos[2],bool visible)
{
    if (!_destroyingContainerNow)
    {
        if (dlgID==HIERARCHY_DLG)
        {
            App::mainWindow->oglSurface->setHierarchyWidth(pos[0]);
            App::mainWindow->oglSurface->setHierarchyEnabled(visible);
        }
        else if (dlgID==BROWSER_DLG)
        {
        }
        else
        {
            CToolDlgWrapper* it=_getDialogWrapper(dlgID);
            if (it!=nullptr)
            {
                it->setPosition(pos[0],pos[1]);
                it->setVisible(visible,parentWindow);
            }
        }
    }
}

void CDlgCont::keyPress(int key)
{
    if (key==CTRL_D_KEY)
        processCommand(OPEN_OBJECT_DLG_OBJECT_SPECIFIC_PART_CMD);
    if (key==CTRL_G_KEY)
        processCommand(OPEN_CALCULATION_DLG_CMD);
}

void CDlgCont::addMenu(VMenu* menu)
{ 
    bool noShapePathEditModeNoSelector=true;
    if ((App::getEditModeType()&SHAPE_EDIT_MODE)||(App::getEditModeType()==PATH_EDIT_MODE))
        noShapePathEditModeNoSelector=false;
    if (App::mainWindow->oglSurface->isPageSelectionActive())
        noShapePathEditModeNoSelector=false;

    if ( (CLibLic::getIntVal(2)==-1)||(CLibLic::getIntVal(2)==1)||(CLibLic::getIntVal(2)==2) )
    {
        menu->appendMenuItem(App::mainWindow->getObjPropToggleViaGuiEnabled()&&noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(OBJECT_DLG),TOGGLE_OBJECT_DLG_CMD,IDSN_OBJECT_PROPERTIES_MENU_ITEM,true);
        menu->appendMenuItem(App::mainWindow->getCalcModulesToggleViaGuiEnabled()&&noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(CALCULATION_DLG),TOGGLE_CALCULATION_DLG_CMD,IDSN_CALCULATION_MODULE_PROPERTIES_MENU_ITEM,true);
        menu->appendMenuSeparator();
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(LUA_SCRIPT_DLG),TOGGLE_LUA_SCRIPT_DLG_CMD,IDSN_SCRIPTS,true);
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(COLLECTION_DLG),TOGGLE_COLLECTION_DLG_CMD,IDSN_COLLECTIONS,true);
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(SELECTION_DLG),TOGGLE_SELECTION_DLG_CMD,IDSN_SELECTION,true);
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(ENVIRONMENT_DLG),TOGGLE_ENVIRONMENT_DLG_CMD,IDSN_ENVIRONMENT,true);
        menu->appendMenuItem(noShapePathEditModeNoSelector&&App::mainWindow->getBrowserToggleViaGuiEnabled(),App::getBrowserEnabled(),TOGGLE_BROWSER_DLG_CMD,IDSN_MODEL_BROWSER,true);
        menu->appendMenuItem(App::mainWindow->getHierarchyToggleViaGuiEnabled(),App::mainWindow->oglSurface->isHierarchyEnabled(),TOGGLE_HIERARCHY_DLG_CMD,IDSN_SCENE_HIERARCHY,true);
    }
    if (CLibLic::getIntVal(2)==0)
    {
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::getBrowserEnabled(),TOGGLE_BROWSER_DLG_CMD,IDSN_MODEL_BROWSER,true);
        menu->appendMenuItem(true,App::mainWindow->oglSurface->isHierarchyEnabled(),TOGGLE_HIERARCHY_DLG_CMD,IDSN_SCENE_HIERARCHY,true);
        menu->appendMenuItem(true,App::mainWindow->dlgCont->isVisible(LAYERS_DLG),TOGGLE_LAYERS_DLG_CMD,IDS_LAYERS,true);
        menu->appendMenuItem(CAuxLibVideo::video_recorderGetEncoderString!=nullptr,App::mainWindow->dlgCont->isVisible(AVI_RECORDER_DLG),TOGGLE_AVI_RECORDER_DLG_CMD,IDSN_AVI_RECORDER,true);
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(SETTINGS_DLG),TOGGLE_SETTINGS_DLG_CMD,IDSN_USER_SETTINGS,true);
    }
    if ( (CLibLic::getIntVal(2)==-1)||(CLibLic::getIntVal(2)==0)||(CLibLic::getIntVal(2)==1)||(CLibLic::getIntVal(2)==2) )
    {
        menu->appendMenuItem(true,App::mainWindow->dlgCont->isVisible(LAYERS_DLG),TOGGLE_LAYERS_DLG_CMD,IDS_LAYERS,true);
        menu->appendMenuItem(CAuxLibVideo::video_recorderGetEncoderString!=nullptr,App::mainWindow->dlgCont->isVisible(AVI_RECORDER_DLG),TOGGLE_AVI_RECORDER_DLG_CMD,IDSN_AVI_RECORDER,true);
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(SETTINGS_DLG),TOGGLE_SETTINGS_DLG_CMD,IDSN_USER_SETTINGS,true);
    }
    if (CLibLic::getIntVal(2)==0)
    {
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::getBrowserEnabled(),TOGGLE_BROWSER_DLG_CMD,IDSN_MODEL_BROWSER,true);
        menu->appendMenuItem(CAuxLibVideo::video_recorderGetEncoderString!=nullptr,App::mainWindow->dlgCont->isVisible(AVI_RECORDER_DLG),TOGGLE_AVI_RECORDER_DLG_CMD,IDSN_AVI_RECORDER,true);
        menu->appendMenuItem(noShapePathEditModeNoSelector,App::mainWindow->dlgCont->isVisible(SETTINGS_DLG),TOGGLE_SETTINGS_DLG_CMD,IDSN_USER_SETTINGS,false);
    }
}

bool CDlgCont::processCommand(int commandID)
{ // Return value is true if the command belonged to dlg menu and was executed

    if (!VThread::isCurrentThreadTheUiThread())
    { // we are NOT in the UI thread.
        // We execute the other commands via the UI thread:
        SUIThreadCommand cmdIn;
        SUIThreadCommand cmdOut;
        cmdIn.cmdId=OPEN_OR_CLOSE_UITHREADCMD;
        cmdIn.intParams.push_back(commandID);
        App::uiThread->executeCommandViaUiThread(&cmdIn,&cmdOut);
    }
    else
    { // We are in the UI thread.
        if (commandID==OPEN_MULTISHAPE_EDITION_DLG_CMD)
        {
            openOrBringToFront(MULTISHAPE_EDITION_DLG);
            return(true);
        }
        if (commandID==CLOSE_MULTISHAPE_EDITION_DLG_CMD)
        {
            close(MULTISHAPE_EDITION_DLG);
            return(true);
        }
        if (commandID==OPEN_GRAPH_DLG_CMD)
        {
            openOrBringToFront(GRAPH_DLG);
            return(true);
        }
        if (commandID==OPEN_CAMERA_DLG_CMD)
        {
            openOrBringToFront(CAMERA_DLG);
            return(true);
        }
        if (commandID==OPEN_LIGHT_DLG_CMD)
        {
            openOrBringToFront(LIGHT_DLG);
            return(true);
        }
        if (commandID==OPEN_MIRROR_DLG_CMD)
        {
            openOrBringToFront(MIRROR_DLG);
            return(true);
        }
        if (commandID==OPEN_SHAPE_DLG_CMD)
        {
            openOrBringToFront(SHAPE_DLG);
            return(true);
        }
        if (commandID==OPEN_PROXIMITY_SENSOR_DLG_CMD)
        {
            openOrBringToFront(PROXIMITY_SENSOR_DLG);
            return(true);
        }
        if (commandID==OPEN_VISION_SENSOR_DLG_CMD)
        {
            openOrBringToFront(VISION_SENSOR_DLG);
            return(true);
        }
        if (commandID==OPEN_MILL_DLG_CMD)
        {
            openOrBringToFront(MILL_DLG);
            return(true);
        }
        if (commandID==OPEN_OBJECT_DLG_CMD)
        {
            openOrBringToFront(OBJECT_DLG);
            return(true);
        }
        if (commandID==OPEN_SHAPE_EDITION_DLG_CMD)
        {
            openOrBringToFront(SHAPE_EDITION_DLG);
            return(true);
        }
        if (commandID==CLOSE_SHAPE_EDITION_DLG_CMD)
        {
            close(SHAPE_EDITION_DLG);
            return(true);
        }
        if (commandID==OPEN_PATH_EDITION_DLG_CMD)
        {
            openOrBringToFront(PATH_EDITION_DLG);
            return(true);
        }
        if (commandID==CLOSE_PATH_EDITION_DLG_CMD)
        {
            close(PATH_EDITION_DLG);
            return(true);
        }
        if (commandID==OPEN_OBJECT_DLG_OBJECT_SPECIFIC_PART_CMD)
        {
            if (App::getEditModeType()==NO_EDIT_MODE)
            {
                openOrBringToFront(OBJECT_DLG);
                VDialog* dlg=getDialog(OBJECT_DLG);
                if (dlg!=nullptr)
                {
                    ((CQDlgObjectDialogContainer*)dlg)->desiredPage=0;
                    ((CQDlgObjectDialogContainer*)dlg)->refresh();
                }
            }
            return(true);
        }
        if ( (commandID==OPEN_SHAPE_EDIT_MODE_TRIANGLE_PART_CMD)||(commandID==OPEN_SHAPE_EDIT_MODE_VERTEX_PART_CMD)||(commandID==OPEN_SHAPE_EDIT_MODE_EDGE_PART_CMD) )
        {
            openOrBringToFront(SHAPE_EDITION_DLG);
            VDialog* dlg=getDialog(SHAPE_EDITION_DLG);
            if (dlg!=nullptr)
            {
                if (commandID==OPEN_SHAPE_EDIT_MODE_TRIANGLE_PART_CMD)
                    ((CQDlgShapeEditionContainer*)dlg)->desiredPage=0;
                if (commandID==OPEN_SHAPE_EDIT_MODE_VERTEX_PART_CMD)
                    ((CQDlgShapeEditionContainer*)dlg)->desiredPage=1;
                if (commandID==OPEN_SHAPE_EDIT_MODE_EDGE_PART_CMD)
                    ((CQDlgShapeEditionContainer*)dlg)->desiredPage=2;
                ((CQDlgShapeEditionContainer*)dlg)->refresh();
            }
            return(true);
        }
        if (commandID==OPEN_FORCE_SENSOR_DLG_CMD)
        {
            openOrBringToFront(FORCE_SENSOR_DLG);
            return(true);
        }
        if (commandID==OPEN_DUMMY_DLG_CMD)
        {
            openOrBringToFront(DUMMY_DLG);
            return(true);
        }
        if (commandID==OPEN_JOINT_DLG_CMD)
        {
            openOrBringToFront(JOINT_DLG);
            return(true);
        }
        if (commandID==OPEN_PATH_DLG_CMD)
        {
            openOrBringToFront(PATH_DLG);
            return(true);
        }
        if (commandID==OPEN_ENVIRONMENT_DLG_CMD)
        {
            openOrBringToFront(ENVIRONMENT_DLG);
            return(true);
        }
        if (commandID==OPEN_FOG_DLG_CMD)
        {
            openOrBringToFront(FOG_DLG);
            return(true);
        }
        if (commandID==TOGGLE_GENERAL_PROPERTIES_DLG_CMD)
        {
            toggle(GENERAL_PROPERTIES_DLG);
            return(true);
        }
        if (commandID==TOGGLE_GRAPH_DLG_CMD)
        {
            toggle(GRAPH_DLG);
            return(true);
        }
        if (commandID==TOGGLE_COLLECTION_DLG_CMD)
        {
            toggle(COLLECTION_DLG);
            return(true);
        }
        if (commandID==TOGGLE_CAMERA_DLG_CMD)
        {
            toggle(CAMERA_DLG);
            return(true);
        }
        if (commandID==TOGGLE_LIGHT_DLG_CMD)
        {
            toggle(LIGHT_DLG);
            return(true);
        }
        if (commandID==TOGGLE_MIRROR_DLG_CMD)
        {
            toggle(MIRROR_DLG);
            return(true);
        }
        if (commandID==TOGGLE_COLLISION_DLG_CMD)
        {
            toggle(COLLISION_DLG);
            return(true);
        }
        if (commandID==TOGGLE_SELECTION_DLG_CMD)
        {
            toggle(SELECTION_DLG);
            return(true);
        }
        if (commandID==TOGGLE_SHAPE_DLG_CMD)
        {
            toggle(SHAPE_DLG);
            return(true);
        }
        if (commandID==TOGGLE_PROXIMITY_SENSOR_DLG_CMD)
        {
            toggle(PROXIMITY_SENSOR_DLG);
            return(true);
        }
        if (commandID==TOGGLE_VISION_SENSOR_DLG_CMD)
        {
            toggle(VISION_SENSOR_DLG);
            return(true);
        }
        if (commandID==TOGGLE_MILL_DLG_CMD)
        {
            toggle(MILL_DLG);
            return(true);
        }
        if (commandID==TOGGLE_FORCE_SENSOR_DLG_CMD)
        {
            toggle(FORCE_SENSOR_DLG);
            return(true);
        }
        if (commandID==TOGGLE_LUA_SCRIPT_DLG_CMD)
        {
            toggle(LUA_SCRIPT_DLG);
            return(true);
        }
        if (commandID==TOGGLE_SETTINGS_DLG_CMD)
        {
            if (CLibLic::getBoolVal(11))
                toggle(SETTINGS_DLG);
            else
            {
                IF_UI_EVENT_CAN_READ_DATA
                {
                    CQDlgOpenGlSettings theDialog(App::mainWindow);

                    theDialog.offscreenContextType=App::userSettings->offscreenContextType;
                    theDialog.fboType=App::userSettings->fboType;
                    theDialog.vboOperation=App::userSettings->vboOperation;
                    theDialog.idleFps=App::userSettings->getIdleFps();
                    theDialog.refresh();
                    if (theDialog.makeDialogModal()!=VDIALOG_MODAL_RETURN_CANCEL)
                    {
                        SSimulationThreadCommand cmd;
                        cmd.cmdId=SET_OPENGLSETTINGS_USERSETTINGSGUITRIGGEREDCMD;
                        cmd.intParams.push_back(theDialog.offscreenContextType);
                        cmd.intParams.push_back(theDialog.fboType);
                        cmd.intParams.push_back(theDialog.vboOperation);

                        cmd.intParams.push_back(App::userSettings->visionSensorsUseGuiThread_windowed);
                        cmd.boolParams.push_back(App::userSettings->oglCompatibilityTweak1);
                        cmd.boolParams.push_back(App::userSettings->useGlFinish);
                        cmd.boolParams.push_back(App::userSettings->useGlFinish_visionSensors);
                        cmd.intParams.push_back(theDialog.idleFps);
                        cmd.boolParams.push_back(App::userSettings->forceFboViaExt);
                        cmd.intParams.push_back(App::userSettings->desiredOpenGlMajor);
                        cmd.intParams.push_back(App::userSettings->desiredOpenGlMinor);
                        App::appendSimulationThreadCommand(cmd);
                    }
                }
            }
            return(true);
        }
        if (commandID==TOGGLE_DUMMY_DLG_CMD)
        {
            toggle(DUMMY_DLG);
            return(true);
        }
        if (commandID==TOGGLE_DISTANCE_DLG_CMD)
        {
            toggle(DISTANCE_DLG);
            return(true);
        }
        if (commandID==TOGGLE_JOINT_DLG_CMD)
        {
            toggle(JOINT_DLG);
            return(true);
        }
        if (commandID==TOGGLE_ENVIRONMENT_DLG_CMD)
        {
            toggle(ENVIRONMENT_DLG);
            return(true);
        }
        if (commandID==TOGGLE_IK_DLG_CMD)
        {
            toggle(IK_DLG);
            return(true);
        }
        if (commandID==TOGGLE_DYNAMICS_DLG_CMD)
        {
            toggle(DYNAMICS_DLG);
            return(true);
        }
        if (commandID==TOGGLE_SIMULATION_DLG_CMD)
        {
            toggle(SIMULATION_DLG);
            return(true);
        }
        if (commandID==TOGGLE_AVI_RECORDER_DLG_CMD)
        {
            toggle(AVI_RECORDER_DLG);
            return(true);
        }
        if (commandID==TOGGLE_HIERARCHY_DLG_CMD)
        {
            toggle(HIERARCHY_DLG);
            return(true);
        }
        if (commandID==OPEN_HIERARCHY_DLG_CMD)
        {
            openOrBringToFront(HIERARCHY_DLG);
            return(true);
        }
        if (commandID==CLOSE_HIERARCHY_DLG_CMD)
        {
            close(HIERARCHY_DLG);
            return(true);
        }
        if (commandID==TOGGLE_BROWSER_DLG_CMD)
        {
            toggle(BROWSER_DLG);
            return(true);
        }
        if (commandID==TOGGLE_LAYERS_DLG_CMD)
        {
            toggle(LAYERS_DLG);
            return(true);
        }
        if (commandID==TOGGLE_DYNAMICS_DLG_CMD)
        {
            toggle(DYNAMICS_DLG);
            return(true);
        }
        if (commandID==TOGGLE_PATH_DLG_CMD)
        {
            toggle(PATH_DLG);
            return(true);
        }
        if (commandID==TOGGLE_OBJECT_DLG_CMD)
        {
            toggle(OBJECT_DLG);
            return(true);
        }
        if (commandID==TOGGLE_CALCULATION_DLG_CMD)
        {
            toggle(CALCULATION_DLG);
            return(true);
        }
        if (commandID==OPEN_CALCULATION_DLG_CMD)
        {
            if (App::getEditModeType()==NO_EDIT_MODE)
                openOrBringToFront(CALCULATION_DLG);
            return(true);
        }
    }
    return(false);
}