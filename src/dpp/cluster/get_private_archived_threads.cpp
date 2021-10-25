/************************************************************************************
 *
 * D++, A Lightweight C++ library for Discord
 *
 * Copyright 2021 Craig Edwards and D++ contributors
 * (https://github.com/brainboxdotcc/DPP/graphs/contributors)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ************************************************************************************/
#include <map>
#include <dpp/discord.h>
#include <dpp/cluster.h>
#include <dpp/discordclient.h>
#include <dpp/discordevents.h>
#include <dpp/message.h>
#include <dpp/cache.h>
#include <dpp/nlohmann/json.hpp>
#include <utility>

namespace dpp {

void cluster::get_private_archived_threads(snowflake channel_id, time_t before_timestamp, uint16_t limit, command_completion_event_t callback) {
	std::string parameters;
	if (before_timestamp) {
		parameters.append("&before=" + std::to_string(before_timestamp));
	}
	if (limit) {
		parameters.append("&limit=" + std::to_string(limit));
	}
	this->post_rest(API_PATH "/channels", std::to_string(channel_id), "/threads/archived/private" + parameters, m_get, "", [callback](json &j, const http_request_completion_t& http) {
		if (callback) {
			channel_map threads;
			for (auto &curr_thread : j) {
				threads[SnowflakeNotNull(&curr_thread, "id")] = channel().fill_from_json(&curr_thread);
			}
			callback(confirmation_callback_t("channel_map", threads, http));
		}
	});
}

};