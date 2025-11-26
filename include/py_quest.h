#pragma once
#include "Headers.h"

using namespace DirectX;

struct QuestData {
	int32_t quest_id = 0;
	uint32_t log_state = 0;
	std::string location = ""; //async fetch DONE
	std::string name = ""; //async fetch DONE
	std::string npc = "";
	uint32_t map_from = 0;
	uint32_t marker_x = 0;
	uint32_t marker_y = 0;
	uint32_t h0024 = 0;
	uint32_t map_to = 0;
	std::string description = ""; //async fetch DONE
	std::string objectives = ""; //async fetch DONE

	bool is_completed = false;
	bool is_current_mission_quest = false;
	bool is_area_primary = false;
	bool is_primary = false;
};

class Quest {
public:
	inline static std::unordered_map<int32_t, std::string> quest_name_map;
	inline static std::unordered_map<int32_t, bool> quest_name_ready_map;

	inline static std::unordered_map<int32_t, std::string> quest_description_map;
	inline static std::unordered_map<int32_t, bool> quest_description_ready_map;

	inline static std::unordered_map<int32_t, std::string> quest_objectives_map;
	inline static std::unordered_map<int32_t, bool> quest_objectives_ready_map;

	inline static std::unordered_map<int32_t, std::string> quest_location_map;
	inline static std::unordered_map<int32_t, bool> quest_location_ready_map;

	inline static std::unordered_map<int32_t, std::string> quest_npc_map;
	inline static std::unordered_map<int32_t, bool> quest_npc_ready_map;

    Quest() {}

    ~Quest() {}

	static void SetActiveQuestId(int32_t quest_id) {
		if (quest_id < 0) return; //invalid quest id
        GW::GameThread::Enqueue([quest_id] {
            GW::QuestMgr::SetActiveQuestId(static_cast<GW::Constants::QuestID>(quest_id));
            });
        
    }

	static int32_t GetActiveQuestId() {
        return static_cast<int32_t>(GW::QuestMgr::GetActiveQuestId());
    }

	static void AbandonQuestId(int32_t quest_id){
		if (quest_id < 0) return; //invalid quest id
        GW::GameThread::Enqueue([quest_id] {
            GW::QuestMgr::AbandonQuestId(static_cast<GW::Constants::QuestID>(quest_id));
            });
    }

	static bool IsQuestCompleted(int32_t quest_id) {
		if (quest_id < 0) return false; //invalid quest id
		GW::Quest* quest = GW::QuestMgr::GetQuest(static_cast<GW::Constants::QuestID>(quest_id));
		if (!quest) return false;
		return quest->IsCompleted();
	}

	static bool IsQuestPrimary(int32_t quest_id) {
		if (quest_id < 0) return false; //invalid quest id
        GW::Quest* quest = GW::QuestMgr::GetQuest(static_cast<GW::Constants::QuestID>(quest_id));
        if (!quest) return false;
        return quest->IsPrimary();
    }

	static bool IsMissionMapQuestAvailable() {
		const auto world_context = GW::GetWorldContext();
		if (!world_context)
			return false;

		const auto count = world_context->mission_objectives.size();
		return count > 0;
	}


	static QuestData GetQuest(int32_t quest_id) {
		QuestData quest_data;

		if (quest_id < 0) {
			if (!IsMissionMapQuestAvailable())
				return quest_data;
			quest_data.quest_id = static_cast<int32_t>(quest_id);
			quest_data.map_from = static_cast<uint32_t>(GW::Map::GetMapID());
			quest_data.map_to = static_cast<uint32_t>(GW::Map::GetMapID());
			quest_data.is_current_mission_quest = true;
			return quest_data;
		};
		
		auto* w = GW::QuestMgr::GetQuestLog();
		if (!w) return quest_data;

		for (auto& q : *w) {
			if (static_cast<int32_t>(q.quest_id) == quest_id) {
				quest_data.quest_id = static_cast<int32_t>(q.quest_id);
				quest_data.log_state = q.log_state;
				
				//quest_data.location = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(q.location);
				//quest_data.name = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(q.name);
				//quest_data.npc = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(q.npc);
				quest_data.map_from = static_cast<int32_t>(q.map_from);
				quest_data.marker_x = static_cast<int32_t>(q.marker.x);
				quest_data.marker_y = static_cast<int32_t>(q.marker.y);
				quest_data.h0024 = q.h0024;
				quest_data.map_to = static_cast<int32_t>(q.map_to);
				//quest_data.description = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(q.description);
				//quest_data.objectives = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(q.objectives);
				quest_data.is_completed = q.IsCompleted();
				quest_data.is_current_mission_quest = q.IsCurrentMissionQuest();
				quest_data.is_area_primary = q.IsAreaPrimary();
				quest_data.is_primary = q.IsPrimary();
				
			}
		}

		return quest_data;

	}

	static bool RequestQuestInfo(int32_t quest_id, bool update_markers = false) {
		if (quest_id < 0) return false; //invalid quest id
		GW::Quest* quest = GW::QuestMgr::GetQuest(static_cast<GW::Constants::QuestID>(quest_id));
		if (!quest) return false;
		return GW::QuestMgr::RequestQuestInfo(quest, update_markers);

	}

	static std::vector<QuestData> GetQuestLog() {
		std::vector<QuestData> quest_log;
		auto* w = GW::QuestMgr::GetQuestLog();
		if (!w) return quest_log;
		for (auto& q : *w) {
			quest_log.push_back(QuestData{
				static_cast<int32_t>(q.quest_id),
				q.log_state,
				"",
				std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(q.name),
				"",
				static_cast<uint32_t>(q.map_from),
				static_cast<uint32_t>(q.marker.x),
				static_cast<uint32_t>(q.marker.y),
				q.h0024,
				static_cast<uint32_t>(q.map_to),
				"",
				"",
				q.IsCompleted(),
				q.IsCurrentMissionQuest(),
				q.IsAreaPrimary(),
				q.IsPrimary()
				});
		}
		return quest_log;
	}

	static std::vector<int32_t> GetQuestLogIds() {
		std::vector<int32_t> quest_log_ids;
		auto* w = GW::QuestMgr::GetQuestLog();
		if (!w) return quest_log_ids;
		for (auto& q : *w) {
			quest_log_ids.push_back(static_cast<int32_t>(q.quest_id));
		}
		return quest_log_ids;
	}

	static std::string custom_WStringToString(const std::wstring& s)
	{
		if (s.empty()) {
			return "Error In Name";
		}

		// --- sanitize embedded nulls and control chars ---
		std::wstring clean;
		clean.reserve(s.size());
		for (wchar_t c : s) {
			if (c >= 32 || c == L'\n' || c == L'\r')
				clean.push_back(c);
		}

		// Determine required size for UTF-8 conversion
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, clean.c_str(),
			static_cast<int>(clean.size()),
			nullptr, 0, nullptr, nullptr);
		if (size_needed <= 0)
			return "Error In Name";

		// Perform the actual conversion
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, clean.c_str(),
			static_cast<int>(clean.size()),
			&strTo[0], size_needed, nullptr, nullptr);

		return strTo;
	}


	static void RequestQuestName(int32_t quest_id) {
		quest_name_ready_map[quest_id] = false;
		//quest_name_map[quest_id].clear();

		auto temp_name = std::make_shared<std::wstring>();

		if (quest_id < 0) {
			if (!IsMissionMapQuestAvailable()) {
				quest_name_map[quest_id] = "No Active Mission";
				quest_name_ready_map[quest_id] = true;
				return;
			}
			quest_name_map[quest_id] = "Mission Objectives";
			quest_name_ready_map[quest_id] = true;
			return;
		};


		std::thread([quest_id, temp_name]() {

			GW::Quest* quest = GW::QuestMgr::GetQuest(static_cast<GW::Constants::QuestID>(quest_id));
			if (!quest) return;

			GW::GameThread::Enqueue([quest, temp_name]() {
				GW::QuestMgr::AsyncGetQuestName(quest, *temp_name);
				});

			auto start = std::chrono::steady_clock::now();
			while (temp_name->empty()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				if (std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::steady_clock::now() - start).count() >= 1000) {
					quest_name_map[quest_id] = "Timeout";
					quest_name_ready_map[quest_id] = true;
					return;
				}
			}

			quest_name_map[quest_id] = custom_WStringToString(*temp_name);
			quest_name_ready_map[quest_id] = true;
			}).detach();
	}


	static bool IsQuestNameReady(int32_t quest_id) {
		auto it = quest_name_ready_map.find(quest_id);
		return it != quest_name_ready_map.end() && it->second;
	}

	static std::string GetQuestName(int32_t quest_id) {
		auto it = quest_name_map.find(quest_id);
		return it != quest_name_map.end() ? it->second : "";
	}

	static void RequestQuestDescription(int32_t quest_id) {
		quest_description_ready_map[quest_id] = false;
		//quest_description_map[quest_id].clear();

		if (quest_id < 0) {
			if (!IsMissionMapQuestAvailable()) {
				quest_description_map[quest_id] = "No Active Mission";
				quest_description_ready_map[quest_id] = true;
				return;
			}
			quest_description_map[quest_id] = "Mission Ongoing";
			quest_description_ready_map[quest_id] = true;
			return;
		};

		auto temp_description = std::make_shared<std::wstring>();

		std::thread([quest_id, temp_description]() {
			GW::Quest* quest = GW::QuestMgr::GetQuest(static_cast<GW::Constants::QuestID>(quest_id));
			if (!quest) return;

			GW::GameThread::Enqueue([quest, temp_description]() {
				GW::QuestMgr::AsyncGetQuestDescription(quest, *temp_description);
				});

			auto start = std::chrono::steady_clock::now();
			while (temp_description->empty()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				if (std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::steady_clock::now() - start).count() >= 1000) {
					quest_description_map[quest_id] = "Timeout";
					quest_description_ready_map[quest_id] = true;
					return;
				}
			}

			quest_description_map[quest_id] = custom_WStringToString(*temp_description);
			quest_description_ready_map[quest_id] = true;
			}).detach();
	}

	static bool IsQuestDescriptionReady(int32_t quest_id) {
		auto it = quest_description_ready_map.find(quest_id);
		return it != quest_description_ready_map.end() && it->second;
	}

	static std::string GetQuestDescription(int32_t quest_id) {
		auto it = quest_description_map.find(quest_id);
		return it != quest_description_map.end() ? it->second : "";
	}

	static void RequestQuestObjectives(int32_t quest_id) {
		constexpr uint32_t OBJECTIVE_FLAG_COMPLETED = 0x2;
		constexpr uint32_t OBJECTIVE_FLAG_BULLET = 0x1;

		quest_objectives_ready_map[quest_id] = false;
		auto temp_objectives = std::make_shared<std::wstring>();

		std::thread([quest_id, temp_objectives]() {
			if (quest_id < 0) {
				if (!IsMissionMapQuestAvailable()) {
					quest_objectives_map[quest_id] = "No Active Mission";
					quest_objectives_ready_map[quest_id] = true;
					return;
				}
				// ---------- MISSION OBJECTIVES ----------
				const auto world_context = GW::GetWorldContext();

				if (!world_context)
					return;

				struct ObjJob {
					uint32_t id{};
					bool bullet{};
					bool completed{};
					std::shared_ptr<std::wstring> text;
				};

				std::vector<ObjJob> jobs;
				jobs.reserve(world_context->mission_objectives.size());

				for (const auto& obj : world_context->mission_objectives) {
					if (!obj.enc_str)
						continue;

					const bool is_bullet = (obj.type & 0x1 /*OBJECTIVE_FLAG_BULLET*/) != 0;
					const bool completed = (obj.type & 0x2 /*OBJECTIVE_FLAG_COMPLETED*/) != 0;

					auto txt = std::make_shared<std::wstring>(L"");
					const std::wstring enc_copy(obj.enc_str);

					GW::GameThread::Enqueue([enc_copy, txt]() {
						GW::QuestMgr::AsyncDecodeAnyEncStr(enc_copy.c_str(), *txt);
						});

					jobs.push_back({ obj.objective_id, is_bullet, completed, txt });
				}

				// Wait for all async decodes to complete
				for (auto& j : jobs) {
					auto start = std::chrono::steady_clock::now();
					while (j.text->empty() || *j.text == L"[PENDING]") {
						std::this_thread::sleep_for(std::chrono::milliseconds(5));
						if (std::chrono::duration_cast<std::chrono::milliseconds>(
							std::chrono::steady_clock::now() - start).count() >= 1000) {
							*j.text = L"Timeout";
							break;
						}
					}
				}

				// Concatenate decoded objectives into a single string
				std::wstring combined;
				combined.reserve(1024);

				for (auto& j : jobs) {
					//combined += L"[ObjID " + std::to_wstring(j.id) + L"] ";
					if (j.bullet)
						combined += (j.completed ? L"{sc}" : L"{s}");
					combined += *j.text;
					combined += L"\n";
				}

				*temp_objectives = std::move(combined);
			}
			else {
				// ---------- NORMAL QUEST OBJECTIVES ----------
				GW::Quest* quest = GW::QuestMgr::GetQuest(static_cast<GW::Constants::QuestID>(quest_id));
				if (!quest)
					return;

				GW::GameThread::Enqueue([quest, temp_objectives]() {
					GW::QuestMgr::AsyncGetQuestObjectives(quest, *temp_objectives);
					});
			}

			// ---------- Wait for main string to be ready ----------
			auto start = std::chrono::steady_clock::now();
			while (temp_objectives->empty()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				if (std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::steady_clock::now() - start).count() >= 1000) {
					quest_objectives_map[quest_id] = "Timeout";
					quest_objectives_ready_map[quest_id] = true;
					return;
				}
			}

			quest_objectives_map[quest_id] = custom_WStringToString(*temp_objectives);
			quest_objectives_ready_map[quest_id] = true;
			}).detach();
	}


	static bool IsQuestObjectivesReady(int32_t quest_id) {
		auto it = quest_objectives_ready_map.find(quest_id);
		return it != quest_objectives_ready_map.end() && it->second;
	}

	static std::string GetQuestObjectives(int32_t quest_id) {
		auto it = quest_objectives_map.find(quest_id);
		return it != quest_objectives_map.end() ? it->second : "";
	}

	static void RequestQuestLocation(int32_t quest_id) {
		quest_location_ready_map[quest_id] = false;
		//quest_location_map[quest_id].clear();
		auto temp_location = std::make_shared<std::wstring>();
		std::thread([quest_id, temp_location]() {
			if (quest_id < 0) {
				if (!IsMissionMapQuestAvailable()) {
					quest_location_map[quest_id] = "No Active Mission";
					quest_location_ready_map[quest_id] = true;
					return;
				}
				const auto area_info = GW::Map::GetCurrentMapInfo();
				if (!area_info)
					return;

				const auto name_id = area_info->name_id ? area_info->name_id : 3;

				wchar_t encoded[8] = { 0 };
				if (!GW::UI::UInt32ToEncStr(name_id, encoded, _countof(encoded)))
					return;  // failed to encode ID - bail early

				const std::wstring encoded_copy(encoded);

				GW::GameThread::Enqueue([encoded_copy, temp_location]() {
					GW::QuestMgr::AsyncDecodeAnyEncStr(encoded_copy.c_str(), *temp_location);
					});
			}
			else {
				GW::Quest* quest = GW::QuestMgr::GetQuest(static_cast<GW::Constants::QuestID>(quest_id));
				if (!quest) return;
				GW::GameThread::Enqueue([quest, temp_location]() {
					GW::QuestMgr::AsyncGetQuestLocation(quest, *temp_location);
					});
			}
			auto start = std::chrono::steady_clock::now();
			while (temp_location->empty()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				if (std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::steady_clock::now() - start).count() >= 1000) {
					quest_location_map[quest_id] = "Timeout";
					quest_location_ready_map[quest_id] = true;
					return;
				}
			}
			quest_location_map[quest_id] = custom_WStringToString(*temp_location);
			quest_location_ready_map[quest_id] = true;
			}).detach();
	}

	static bool IsQuestLocationReady(int32_t quest_id) {
		auto it = quest_location_ready_map.find(quest_id);
		return it != quest_location_ready_map.end() && it->second;
	}

	static std::string GetQuestLocation(int32_t quest_id) {
		auto it = quest_location_map.find(quest_id);
		return it != quest_location_map.end() ? it->second : "";
	}

	static void RequestQuestNPC(int32_t quest_id) {
		quest_npc_ready_map[quest_id] = false;
		//quest_npc_map[quest_id].clear();

		if (quest_id < 0) {
			if (!IsMissionMapQuestAvailable()) {
				quest_npc_map[quest_id] = "No Active Mission";
				quest_npc_ready_map[quest_id] = true;
				return;
			}
			quest_npc_map[quest_id] = "Mission Ongoing";
			quest_npc_ready_map[quest_id] = true;
			return;
		};


		auto temp_npc = std::make_shared<std::wstring>();
		std::thread([quest_id, temp_npc]() {
			GW::Quest* quest = GW::QuestMgr::GetQuest(static_cast<GW::Constants::QuestID>(quest_id));
			if (!quest) return;
			GW::GameThread::Enqueue([quest, temp_npc]() {
				GW::QuestMgr::AsyncGetQuestNPC(quest, *temp_npc);
				});
			auto start = std::chrono::steady_clock::now();
			while (temp_npc->empty()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				if (std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::steady_clock::now() - start).count() >= 1000) {
					quest_npc_map[quest_id] = "Timeout";
					quest_npc_ready_map[quest_id] = true;
					return;
				}
			}
			quest_npc_map[quest_id] = custom_WStringToString(*temp_npc);
			quest_npc_ready_map[quest_id] = true;
			}).detach();
	}

	static bool IsQuestNPCReady(int32_t quest_id) {
		auto it = quest_npc_ready_map.find(quest_id);
		return it != quest_npc_ready_map.end() && it->second;
	}

	static std::string GetQuestNPC(int32_t quest_id) {
		auto it = quest_npc_map.find(quest_id);
		return it != quest_npc_map.end() ? it->second : "";
	}

};

