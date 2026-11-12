#!/usr/bin/env python3
"""让 esp-wifi-connect 支持隐藏 SSID。

原逻辑：扫描到的 AP 按 SSID 和已保存列表逐个比对，匹配上才进连接队列。
隐藏网络的 beacon 不含 SSID，扫描结果里 ap_record.ssid 为空，永远匹配不上，
于是 connect_queue_ 一直为空 → 无限重扫（日志：No AP found, next scan in ...）。

补丁：扫不到、但本地确实保存过 SSID 时，构造一条 channel=0 的记录直接连。
channel=0 会让 StartConnect() 走 WIFI_ALL_CHANNEL_SCAN 分支，
驱动按配置里的 SSID 发定向 probe request，隐藏网络照样能连上。
"""
import pathlib
import sys

TARGET = pathlib.Path("managed_components/78__esp-wifi-connect/wifi_station.cc")

OLD = """    if (connect_queue_.empty()) {
        if (last_scan_used_saved_channels_) {"""

NEW = """    if (connect_queue_.empty() && !ssid_list.empty()) {
        // [本地补丁] 隐藏 SSID 兜底：扫描结果里没有匹配项，但用户配置过网络。
        // channel=0 -> StartConnect() 走 WIFI_ALL_CHANNEL_SCAN，驱动发定向 probe。
        const SsidItem& hidden_item = ssid_list.front();
        WifiApRecord hidden_record = {
            .ssid = hidden_item.ssid,
            .password = hidden_item.password,
            .channel = 0,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .bssid = {0}
        };
        ESP_LOGW(TAG, "扫描未匹配到已保存 SSID（隐藏网络？），直接连接: %s", hidden_item.ssid.c_str());
        connect_queue_.push_back(hidden_record);
        StartConnect();
        return;
    }

    if (connect_queue_.empty()) {
        if (last_scan_used_saved_channels_) {"""


def main():
    if not TARGET.exists():
        print("找不到 %s，组件还没拉下来？" % TARGET)
        return 1
    src = TARGET.read_text(encoding="utf-8")
    if "[本地补丁] 隐藏 SSID 兜底" in src:
        print("补丁已存在，跳过")
        return 0
    if OLD not in src:
        print("找不到插入点，组件版本变了，需要重新对照源码")
        return 1
    TARGET.write_text(src.replace(OLD, NEW, 1), encoding="utf-8")
    print("补丁已应用:", TARGET)
    return 0


if __name__ == "__main__":
    sys.exit(main())
