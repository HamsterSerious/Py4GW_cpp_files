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
		.def("set_active_quest_id", &Quest::SetActiveQuestId, py::arg("quest_id"))
		.def("get_active_quest_id", &Quest::GetActiveQuestId)
		.def("abandon_quest_id", &Quest::AbandonQuestId, py::arg("quest_id"))
		.def("is_quest_completed", &Quest::IsQuestCompleted, py::arg("quest_id"))
		.def("is_quest_primary", &Quest::IsQuestPrimary, py::arg("quest_id"))
		.def("get_quest_data", &Quest::GetQuest, py::arg("quest_id"))
		.def("get_quest_log", &Quest::GetQuestLog)
		.def("request_quest_info", &Quest::RequestQuestInfo, py::arg("quest_id"), py::arg("update_markers") = false);
}



