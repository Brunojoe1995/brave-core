/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_AI_CHAT_AI_CHAT_API_H_
#define BRAVE_COMPONENTS_AI_CHAT_AI_CHAT_API_H_

#include <memory>
#include <string>

#include "brave/components/api_request_helper/api_request_helper.h"

namespace network {
class SharedURLLoaderFactory;
}  // namespace network

class AIChatAPI {
 public:
  explicit AIChatAPI(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);
  AIChatAPI(const AIChatAPI&) = delete;
  AIChatAPI& operator=(const AIChatAPI&) = delete;
  ~AIChatAPI();

  using ResponseCallback = base::RepeatingCallback<void(const std::string&)>;
  using CompletionCallback =
      base::OnceCallback<void(bool success, int response_code)>;

  void QueryPrompt(
      std::unique_ptr<api_request_helper::StreamCallbacks> callbacks,
      const std::string& prompt);
  bool IsRequestInProgress();

 private:
  base::Value::Dict CreateApiParametersDict(const std::string& prompt);

  api_request_helper::APIRequestHelper api_request_helper_;
};

#endif  // BRAVE_COMPONENTS_AI_CHAT_AI_CHAT_API_H_
