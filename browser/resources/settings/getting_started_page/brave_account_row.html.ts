/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at https://mozilla.org/MPL/2.0/. */

import './brave_account_create_dialog.js'
import './brave_account_entry_dialog.js'
import './brave_account_forgot_password_dialog.js'
import './brave_account_sign_in_dialog.js'
import { html, nothing } from '//resources/lit/v3_0/lit.rollup.js'
import { DialogType, SettingsBraveAccountRow } from './brave_account_row.js'
 
export function getHtml(this: SettingsBraveAccountRow) {
  return html`<!--_html_template_start_-->
    <div class="row">
      <div class="circle">
        <div class="logo"></div>
      </div>
      <div class="texts">
        <div class="text-top">Sign in or create a Brave account</div>
        <div class="text-bottom">$i18n{braveSyncBraveAccountDesc}</div>
      </div>
      <div class="button-wrapper">
        <leo-button class="button" size="small" @click=${() => this.dialogType = DialogType.ENTRY}>
          Get started
        </leo-button>
      </div>
    </div>
    ${(() => {
      switch(this.dialogType) {
        case DialogType.NONE:
          return nothing
        case DialogType.ENTRY:
          return html`
            <settings-brave-account-entry-dialog
              @close=${() => this.dialogType = DialogType.NONE}
              @create-button-clicked=${() => this.dialogType = DialogType.CREATE}
              @sign-in-button-clicked=${() => this.dialogType = DialogType.SIGN_IN}
              @self-custody-button-clicked=${() => this.dialogType = DialogType.NONE}>
            </settings-brave-account-entry-dialog>
          `
        case DialogType.CREATE:
          return html`
            <settings-brave-account-create-dialog
              @close=${() => this.dialogType = DialogType.NONE}
              @back-button-clicked=${this.onBackButtonClicked}>
            </settings-brave-account-create-dialog>
          `
        case DialogType.SIGN_IN:
          return html`
            <settings-brave-account-sign-in-dialog
              @close=${() => this.dialogType = DialogType.NONE}
              @back-button-clicked=${this.onBackButtonClicked}
              @forgot-password-button-clicked=${() => this.dialogType = DialogType.FORGOT_PASSWORD}>
            </settings-brave-account-sign-in-dialog>
          `
        case DialogType.FORGOT_PASSWORD:
          return html`
            <settings-brave-account-forgot-password-dialog
              @close=${() => this.dialogType = DialogType.NONE}
              @back-button-clicked=${this.onBackButtonClicked}
              @cancel-button-clicked=${this.onBackButtonClicked}>
            </settings-brave-account-forgot-password-dialog>
          `
      }
    })()}
  <!--_html_template_end_-->`
}
