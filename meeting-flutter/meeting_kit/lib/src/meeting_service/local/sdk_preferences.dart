// Copyright (c) 2022 NetEase, Inc. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.
part of meeting_service;

class LoginInfo {
  final String appKey;
  final String userUuid;
  final String userToken;

  LoginInfo(this.appKey, this.userUuid, this.userToken);
}

class SDKPreferences {
  static const String _keyDeviceId = 'meeting_sdk_deviceId';
  static const String _keyLoginInfo = 'meeting_sdk_login_info';
  static const String _keyAccountId = 'account';
  static const String _keyAccountToken = 'token';
  static const String _keyAppKey = 'appkey';

  static Future<void> setDeviceId(String deviceId) async {
    await _setString(_keyDeviceId, deviceId);
  }

  static Future<String?> getDeviceId() {
    return _getString(_keyDeviceId, '');
  }

  static Future<void> setLoginInfo(LoginInfo? loginInfo) async {
    if (loginInfo == null) {
      await _setString(_keyLoginInfo, null);
    } else {
      await _setString(
          _keyLoginInfo,
          json.encode({
            _keyAppKey: loginInfo.appKey,
            _keyAccountId: loginInfo.userUuid,
            _keyAccountToken: loginInfo.userToken,
          }));
    }
  }

  static Future<LoginInfo?> getLoginInfo() async {
    final cache = await _getString(_keyLoginInfo, null);
    if (cache != null) {
      final loginInfoJson = json.decode(cache);
      if (loginInfoJson is Map) {
        return LoginInfo(
          loginInfoJson[_keyAppKey] as String,
          loginInfoJson[_keyAccountId] as String,
          loginInfoJson[_keyAccountToken] as String,
        );
      }
    }
    return null;
  }

  static SharedPreferences? _prefs;

  static Future<SharedPreferences> _ensureSharedPreferences() async {
    _prefs ??= await SharedPreferences.getInstance();
    return _prefs!;
  }

  static Future<bool> _setString(String key, String? value) async {
    await _ensureSharedPreferences();
    if (value == null) {
      return await _prefs!.remove(key);
    } else {
      return await _prefs!.setString(key, value);
    }
  }

  static Future<String?> _getString(String key, [String? fallback]) async {
    await _ensureSharedPreferences();
    return _prefs!.getString(key) ?? fallback;
  }
}
