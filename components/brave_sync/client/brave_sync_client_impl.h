/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_CLIENT_IMPL_H
#define BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_CLIENT_IMPL_H

#include "brave/components/brave_sync/client/brave_sync_client.h"
#include "base/macros.h"
#include "base/scoped_observer.h"
#include "components/prefs/pref_change_registrar.h"
#include "extensions/browser/extension_registry_observer.h"

class Profile;

namespace extensions {
class BraveSyncEventRouter;
}

namespace brave_sync {

namespace prefs {
  class Prefs;
}

using extensions::Extension;
using extensions::ExtensionRegistry;
using extensions::ExtensionRegistryObserver;
using extensions::UnloadedExtensionReason;

class BraveSyncClientImpl : public BraveSyncClient,
                            public ExtensionRegistryObserver {
 public:
  BraveSyncClientImpl(SyncMessageHandler* handler, Profile* profile);
  ~BraveSyncClientImpl() override;

  // BraveSyncClient overrides

  // BraveSync to Browser messages
  SyncMessageHandler* sync_message_handler() override;

  // Browser to BraveSync messages
  void SendGotInitData(const Uint8Array& seed, const Uint8Array& device_id,
    const client_data::Config& config, const std::string& sync_words) override;
  void SendFetchSyncRecords(
    const std::vector<std::string> &category_names, const base::Time &startAt,
    const int max_records) override;
  void SendFetchSyncDevices() override ;
  void SendResolveSyncRecords(
      const std::string& category_name,
      std::unique_ptr<SyncRecordAndExistingList> records) override;
  void SendSyncRecords(const std::string& category_name,
    const RecordsList &records) override;
  void SendDeleteSyncUser() override;
  void SendDeleteSyncCategory(const std::string& category_name) override;
  void SendGetBookmarksBaseOrder(const std::string& device_id,
                                 const std::string& platform) override;
  void SendGetBookmarkOrder(const std::string& prev_order,
                            const std::string& next_order,
                            const std::string& parent_order) override;
  void NeedSyncWords(const std::string& seed) override;

 private:
  void OnExtensionInitialized() override;

  // ExtensionRegistryObserver:
  void OnExtensionReady(content::BrowserContext* browser_context,
                        const extensions::Extension* extension) override;
  void OnExtensionLoaded(content::BrowserContext* browser_context,
                         const Extension* extension) override;
  void OnExtensionUnloaded(content::BrowserContext* browser_context,
                           const Extension* extension,
                           UnloadedExtensionReason reason) override;
  void LoadOrUnloadExtension(bool load);
  void OnExtensionSystemReady();
  void OnProfilePreferenceChanged();

  SyncMessageHandler* handler_;  // not owned
  Profile* profile_;  // not owned
  std::unique_ptr<brave_sync::prefs::Prefs> sync_prefs_;
  bool extension_loaded_;

  std::unique_ptr<extensions::BraveSyncEventRouter> brave_sync_event_router_;

  // Registrar used to monitor the profile prefs.
  PrefChangeRegistrar profile_pref_change_registrar_;

  ScopedObserver<ExtensionRegistry, ExtensionRegistryObserver>
    extension_registry_observer_;

  DISALLOW_COPY_AND_ASSIGN(BraveSyncClientImpl);
};

} // namespace brave_sync

#endif // BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_CLIENT_IMPL_H
