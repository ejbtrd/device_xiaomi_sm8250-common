# Remove old hals
rm -rf hardware/qcom-caf/sm8250/{audio,display,media}
rm -rf vendor/qcom/opensource/commonsys-intf/display

# Clone LOS hals
git clone https://github.com/LineageOS/android_hardware_qcom_audio/ -b lineage-18.1-caf-sm8250 hardware/qcom-caf/sm8250/audio
git clone https://github.com/LineageOS/android_hardware_qcom_display/ -b lineage-18.1-caf-sm8250 hardware/qcom-caf/sm8250/display
git clone https://github.com/LineageOS/android_hardware_qcom_media/ -b lineage-18.1-caf-sm8250 hardware/qcom-caf/sm8250/media
git clone https://github.com/LineageOS/android_vendor_qcom_opensource_display-commonsys-intf vendor/qcom/opensource/commonsys-intf/display

repo sync --force-sync hardware/qcom-caf/common
