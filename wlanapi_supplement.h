// vim: set sw=4 ts=4 ft=cpp et fenc=utf-8:

#ifndef WLANAPI_SUPPLEMENT_H
#define WLANAPI_SUPPLEMENT_H

#include <windot11.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _WLAN_HOSTED_NETWORK_OPCODE {
  wlan_hosted_network_opcode_connection_settings,
  wlan_hosted_network_opcode_security_settings,
  wlan_hosted_network_opcode_station_profile,
  wlan_hosted_network_opcode_enable
} WLAN_HOSTED_NETWORK_OPCODE, *PWLAN_HOSTED_NETWORK_OPCODE;

typedef struct _WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS {
  DOT11_SSID hostedNetworkSSID;
  DWORD      dwMaxNumberOfPeers;
} WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS, *PWLAN_HOSTED_NETWORK_CONNECTION_SETTINGS;

typedef enum _WLAN_HOSTED_NETWORK_REASON {
  wlan_hosted_network_reason_success = 0,
  wlan_hosted_network_reason_unspecified,
  wlan_hosted_network_reason_bad_parameters,
  wlan_hosted_network_reason_service_shutting_down,
  wlan_hosted_network_reason_insufficient_resources,
  wlan_hosted_network_reason_elevation_required,
  wlan_hosted_network_reason_read_only,
  wlan_hosted_network_reason_persistence_failed,
  wlan_hosted_network_reason_crypt_error,
  wlan_hosted_network_reason_impersonation,
  wlan_hosted_network_reason_stop_before_start,
  wlan_hosted_network_reason_interface_available,
  wlan_hosted_network_reason_interface_unavailable,
  wlan_hosted_network_reason_miniport_stopped,
  wlan_hosted_network_reason_incompatible_connection_started,
  wlan_hosted_network_reason_incompatible_connection_stopped,
  wlan_hosted_network_reason_user_action,
  wlan_hosted_network_reason_client_abort,
  wlan_hosted_network_reason_ap_start_failed,
  wlan_hosted_network_reason_delete_later,
  wlan_hosted_network_reason_wifi_off,
  wlan_hosted_network_reason_interface_removed,
  wlan_hosted_network_reason_coexistence,
  wlan_hosted_network_reason_device_change
} WLAN_HOSTED_NETWORK_REASON, *PWLAN_HOSTED_NETWORK_REASON;


DWORD WINAPI WlanHostedNetworkSetProperty(
  HANDLE                         hClientHandle,
  WLAN_HOSTED_NETWORK_OPCODE     OpCode,
  DWORD                          dwDataSize,
  PVOID                          pvData,
  PWLAN_HOSTED_NETWORK_REASON    pFailReason,
  PVOID                          pvReserved
);

DWORD WINAPI WlanHostedNetworkSetSecondaryKey(
  HANDLE                         hClientHandle,
  DWORD                          dwKeyLength,
  PUCHAR                         pucKeyData,
  BOOL                           bIsPassPhrase,
  BOOL                           bPersistent,
  PWLAN_HOSTED_NETWORK_REASON    pFailReason,
  PVOID                          pvReserved
);

DWORD WINAPI WlanHostedNetworkStartUsing(
  HANDLE                         hClientHandle,
  PWLAN_HOSTED_NETWORK_REASON    pFailReason,
  PVOID                          pvReserved
);

#ifdef __cplusplus
}
#endif

#endif // WLANAPI_SUPPLEMENT_H
