﻿// Copyright (c) 2022 NetEase, Inc. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include "main_thread.h"
#include "misc_thread.h"
#include "resource.h"

#include "main/setup_data.h"

#include "window/msg_box.h"
#include "window/setup_wnd.h"

static MiscThread* misc_thread_global_ = NULL;

bool CheckYixinSingletonRun() {
    // nimͬʱжͨ
    std::vector<int> exe;
    ;
    return CSetupData::FindAppProcessID(CSetupData::GetProcessName(), exe);
}

// bool CheckSingletonRun()  //ֵmutexɹtrue
//{
//	// ֻһ
//#ifdef _DEBUG
//	const wchar_t kMutexName[] = L"RunOnlyOneYinxinUnInstallInstanceDebug";
//#else
//	const wchar_t kMutexName[] = L"RunOnlyOneYinxinUnInstallInstance";
//#endif
//	HANDLE mutex = ::CreateMutex(NULL, TRUE, kMutexName);
//	if (mutex == NULL || ERROR_ALREADY_EXISTS == ::GetLastError())
//	{
//		if (mutex != NULL)
//		{
//			::CloseHandle(mutex);
//		}
//		return false;
//	}
//	return true;
//}

void MainThread::Init() {
    nbase::ThreadManager::RegisterThread(threading::kThreadUI);

    //#ifdef _DEBUG
    //	ui::GlobalManager::Startup(nbase::win32::GetCurrentModuleDirectory() + L"res/", ui::CreateControlCallback());
    //#else
    HRSRC hrsc = FindResource(NULL, MAKEINTRESOURCE(IDR_ZIP_RES), L"ZIP");
    HGLOBAL hG = LoadResource(NULL, hrsc);
    DWORD dwSize = SizeofResource(NULL, hrsc);
    if (hG && dwSize > 0) {
        ui::GlobalManager::OpenResZip(hG, dwSize, "");
    }
    ui::GlobalManager::Startup(L"res/", ui::CreateControlCallback());
    //#endif

    StartMiscThread();
    ShowSetupWnd();
}

void MainThread::Cleanup() {
    StopMiscThread();
    ui::GlobalManager::Shutdown();
    SetThreadWasQuitProperly(true);
    nbase::ThreadManager::UnregisterThread();
}
void MainThread::StartMiscThread() {
    if (NULL == misc_thread_global_) {
        misc_thread_global_ = new MiscThread(threading::kThreadMiscGlobal, "misc thread (global)");
        misc_thread_global_->Start();
    }
}
void MainThread::StopMiscThread() {
    if (misc_thread_global_) {
        misc_thread_global_->Stop();
        delete misc_thread_global_;
        misc_thread_global_ = NULL;
    }
}
void MainThread::ShowSetupWnd() {
    bool yixin_running = CheckYixinSingletonRun();
    if (yixin_running) {
        MsgForm::MsgboxCallback msgbox_cb = [this](MsgBoxRet ret) {
            if (ret == kMsgBtn2) {
                ShowSetupWnd();
            } else {
                this->EndSession();
            }
        };
        MsgBox()
            ->SetTitle(L"STRING_TIPS")
            ->SetInfor(L"STRING_YIXIN_RUNNING_TIP")
            ->SetIcon(kMsgIconWarn)
            ->AddButton(L"STRING_NO", true)
            ->AddButton(L"STRING_RETRY")
            ->AsynShow(NULL, msgbox_cb);
    } else {
        SetupForm* setup_form = new SetupForm(install_path_);
        setup_form->Create(NULL, L"nim_uninstall_form", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
        setup_form->CenterWindow();
        setup_form->ShowWindow(true);
        ::SetForegroundWindow(setup_form->GetHWND());
    }
}
void MainThread::EndSession() {
    StdClosure cb = []() { ::PostQuitMessage(0); };
    nbase::ThreadManager::PostTask(threading::kThreadUI, cb);
}
