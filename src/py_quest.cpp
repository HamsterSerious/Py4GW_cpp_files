#pragma once
#include "Headers.h"

using namespace DirectX;

void BindQuestData(py::module_& m) {
	py::class_<QuestData>(m, "QuestData")
		.def(py::init<>())
		.def_readwrite("quest_id", &QuestData::quest_id)
		.def_readwrite("log_state", &QuestData::log_state)
		.def_readwrite("location", &QuestData::location)
		.def_readwrite("name", &QuestData::name)
		.def_readwrite("npc", &QuestData::npc)
		.def_readwrite("map_from", &QuestData::map_from)
		.def_readwrite("marker_x", &QuestData::marker_x)
		.def_readwrite("marker_y", &QuestData::marker_y)
		.def_readwrite("h0024", &QuestData::h0024)
		.def_readwrite("map_to", &QuestData::map_to)
		.def_readwrite("description", &QuestData::description)
		.def_readwrite("objectives", &QuestData::objectives)
		.def_readwrite("is_completed", &QuestData::is_completed)
		.def_readwrite("is_current_mission_quest", &QuestData::is_current_mission_quest)
		.def_readwrite("is_area_primary", &QuestData::is_area_primary)
		.def_readwrite("is_primary", &QuestData::is_primary);
}


PYBIND11_EMBEDDED_MODULE(PyQuest, m) {
	BindQuestData(m);
	py::class_<Quest>(m, "PyQuest")
		.def(py::init<>())
		.def_static("set_active_quest_id", &Quest::SetActiveQuestId, py::arg("quest_id"))
		.def_static("get_active_quest_id", &Quest::GetActiveQuestId)
		.def_static("abandon_quest_id", &Quest::AbandonQuestId, py::arg("quest_id"))
		.def_static("is_quest_completed", &Quest::IsQuestCompleted, py::arg("quest_id"))
		.def_static("is_quest_primary", &Quest::IsQuestPrimary, py::arg("quest_id"))
		.def_static("is_mission_map_quest_available", &Quest::IsMissionMapQuestAvailable)
		.def_static("get_quest_data", &Quest::GetQuest, py::arg("quest_id"))
		.def_static("get_quest_log", &Quest::GetQuestLog)
		.def_static("get_quest_log_ids", &Quest::GetQuestLogIds)
		.def_static("request_quest_info", &Quest::RequestQuestInfo, py::arg("quest_id"), py::arg("update_markers") = false)
		.def_static("request_quest_name", &Quest::RequestQuestName, py::arg("quest_id"))
		.def_static("is_quest_name_ready", &Quest::IsQuestNameReady, py::arg("quest_id"))
		.def_static("get_quest_name", &Quest::GetQuestName, py::arg("quest_id"))
		.def_static("request_quest_description", &Quest::RequestQuestDescription, py::arg("quest_id"))
		.def_static("is_quest_description_ready", &Quest::IsQuestDescriptionReady, py::arg("quest_id"))
		.def_static("get_quest_description", &Quest::GetQuestDescription, py::arg("quest_id"))
		.def_static("request_quest_objectives", &Quest::RequestQuestObjectives, py::arg("quest_id"))
		.def_static("is_quest_objectives_ready", &Quest::IsQuestObjectivesReady, py::arg("quest_id"))
		.def_static("get_quest_objectives", &Quest::GetQuestObjectives, py::arg("quest_id"))
		.def_static("request_quest_location", &Quest::RequestQuestLocation, py::arg("quest_id"))
		.def_static("is_quest_location_ready", &Quest::IsQuestLocationReady, py::arg("quest_id"))
		.def_static("get_quest_location", &Quest::GetQuestLocation, py::arg("quest_id"))
		.def_static("request_quest_npc", &Quest::RequestQuestNPC, py::arg("quest_id"))
		.def_static("is_quest_npc_ready", &Quest::IsQuestNPCReady, py::arg("quest_id"))
		.def_static("get_quest_npc", &Quest::GetQuestNPC, py::arg("quest_id"));
}



