/*
 * Copyright (C) 2019-2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "FingerprintInscreenService"

#include "FingerprintInscreen.h"
#include <android-base/logging.h>
#include <hardware_legacy/power.h>
#include <cmath>
#include <fstream>

#define COMMAND_NIT 10
#define PARAM_NIT_FOD 1
#define PARAM_NIT_NONE 0

#define TOUCH_FOD_ENABLE 10

#define BRIGHTNESS_PATH "/sys/class/backlight/panel0-backlight/brightness"

#define DISPPARAM_PATH "/sys/devices/platform/soc/ae00000.qcom,mdss_mdp/drm/card0/card0-DSI-1/disp_param"
#define DISPPARAM_HBM_FOD_ON "0x20000"
#define DISPPARAM_HBM_FOD_OFF "0xE0000"
#define FINGERPRINT_ERROR_VENDOR 8

namespace vendor {
namespace lineage {
namespace biometrics {
namespace fingerprint {
namespace inscreen {
namespace V1_0 {
namespace implementation {

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;

    file >> result;
    return file.fail() ? def : result;
}

FingerprintInscreen::FingerprintInscreen() {
    mXiaomiDisplayFeatureService = IDisplayFeature::getService();
    mTouchFeatureService = ITouchFeature::getService();
    mXiaomiFingerprintService = IXiaomiFingerprint::getService();
}

Return<void> FingerprintInscreen::onStartEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::onFinishEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::onPress() {
    acquire_wake_lock(PARTIAL_WAKE_LOCK, LOG_TAG);
    mXiaomiFingerprintService->extCmd(COMMAND_NIT, PARAM_NIT_FOD);
    return Void();
}

Return<void> FingerprintInscreen::onRelease() {
    mXiaomiFingerprintService->extCmd(COMMAND_NIT, PARAM_NIT_NONE);
    release_wake_lock(LOG_TAG);
    return Void();
}

Return<void> FingerprintInscreen::onShowFODView() {
    mTouchFeatureService->setTouchMode(TOUCH_FOD_ENABLE, 1);
    mXiaomiDisplayFeatureService->setFeature(0, 17, 1, 1);
    return Void();
}

Return<void> FingerprintInscreen::onHideFODView() {
    mTouchFeatureService->resetTouchMode(TOUCH_FOD_ENABLE);
    mXiaomiDisplayFeatureService->setFeature(0, 17, 0, 1);
    return Void();
}

Return<bool> FingerprintInscreen::handleAcquired(int32_t acquiredInfo, int32_t vendorCode) {
    LOG(ERROR) << "acquiredInfo: " << acquiredInfo << ", vendorCode: " << vendorCode;
    return false;
}

Return<bool> FingerprintInscreen::handleError(int32_t error, int32_t vendorCode) {
    LOG(ERROR) << "error: " << error << ", vendorCode: " << vendorCode;
    return error == FINGERPRINT_ERROR_VENDOR && vendorCode == 6;
}

Return<void> FingerprintInscreen::setLongPressEnabled(bool) {
    return Void();
}

Return<int32_t> FingerprintInscreen::getDimAmount(int32_t) {
    float alpha;
    int realBrightness = get(BRIGHTNESS_PATH, 0);
    if (realBrightness > 9) {
    alpha = (255 + ((-8.08071) * pow(realBrightness, 0.45)));
    } else {
    alpha = (255 + ((-10.08071) * pow(realBrightness, 0.45)));
    }
    if(alpha < 0.82)
    alpha+=0.1;
    return alpha;
}

Return<bool> FingerprintInscreen::shouldBoostBrightness() {
    return false;
}

Return<void> FingerprintInscreen::setCallback(const sp<IFingerprintInscreenCallback>& /* callback */) {
    return Void();
}

Return<int32_t> FingerprintInscreen::getPositionX() {
    return FOD_POS_X;
}

Return<int32_t> FingerprintInscreen::getPositionY() {
    return FOD_POS_Y;
}

Return<int32_t> FingerprintInscreen::getSize() {
    return FOD_SIZE;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace inscreen
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace lineage
}  // namespace vendor
