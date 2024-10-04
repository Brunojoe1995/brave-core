// Copyright (c) 2020 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import '../settings_shared.css.js';

import 'chrome://resources/cr_elements/cr_dialog/cr_dialog.js';
import 'chrome://resources/cr_elements/cr_button/cr_button.js';
import 'chrome://resources/cr_elements/cr_icon_button/cr_icon_button.js';

import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import {I18nMixin, I18nMixinInterface} from 'chrome://resources/cr_elements/i18n_mixin.js'
import {BaseMixin} from '../base_mixin.js'
import {BraveSyncBrowserProxy} from './brave_sync_browser_proxy.js';
import {getTemplate} from './brave_account_dialog.html.js'

/**
 * @fileoverview
 * 'settings-brave-account-dialog'...
 */

const SettingsBraveAccountDialogElementBase =
  I18nMixin(BaseMixin(PolymerElement)) as {
    new(): PolymerElement & I18nMixinInterface
  }

export class SettingsBraveAccountDialogElement extends SettingsBraveAccountDialogElementBase {
  static get is() {
    return 'settings-brave-account-dialog'
  }

  static get template() {
    return getTemplate()
  }

  static get properties() {
    return {
      /**
       * 'create' | 'signin'
       */
      codeType: {
        type: String,
        value: 'signin',
        notify: true
      },
    };
  }

  private codeType: 'create' | 'signin' | null;

  syncBrowserProxy_: BraveSyncBrowserProxy = BraveSyncBrowserProxy.getInstance();

  isCodeType(askingType: string) {
    return (this.codeType === askingType)
  }

  handleClose_() {
    this.codeType = null
  }
}

customElements.define(
  SettingsBraveAccountDialogElement.is, SettingsBraveAccountDialogElement)
