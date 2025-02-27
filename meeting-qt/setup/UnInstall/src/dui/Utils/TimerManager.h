﻿// Copyright (c) 2022 NetEase, Inc. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef UI_ANIMATION_ANIMATIONTIMER_H_
#define UI_ANIMATION_ANIMATIONTIMER_H_

#pragma once

#include "base/base_export.h"
#include "base/std_helper/weak_callback.h"

namespace ui {

struct TIMERINFO {
    typedef std::function<void()> TimerCallback;

    bool operator<(const TIMERINFO& timerInfo) const { return dwTimeEnd.QuadPart > timerInfo.dwTimeEnd.QuadPart; }

    TIMERINFO()
        : uPause(0)
        , iRepeatTime(0) {
        dwTimeEnd.QuadPart = 0;
    }

    TimerCallback timerCallback;
    LONGLONG uPause;                          //ʱ
    LARGE_INTEGER dwTimeEnd;                  //ʱʱ䣨λ1ms
    std::weak_ptr<nbase::WeakFlag> weakFlag;  //ظ
    int iRepeatTime;                          //ظ
};

class TimerManager {
public:
    static TimerManager* GetInstance() {
        static TimerManager timerManager;
        return &timerManager;
    }

    const static int REPEAT_FOREVER = -1;
    bool AddCancelableTimer(const std::weak_ptr<nbase::WeakFlag>& weakFlag, const TIMERINFO::TimerCallback& callback, UINT uElapse, int iRepeatTime);

private:
    TimerManager();
    static LRESULT CALLBACK WndProcThunk(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    static void CALLBACK TimeCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

    void Poll();

private:
    std::priority_queue<TIMERINFO> m_aTimers;
    static HWND m_hideWnd;
    LARGE_INTEGER m_liPerfFreq;
    bool m_isMinPause = true;
    UINT m_oldTimerId = 0;
};

}  // namespace ui

#endif  // UI_ANIMATION_ANIMATIONTIMER_H_
