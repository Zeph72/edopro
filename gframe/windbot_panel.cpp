#include <IGUIComboBox.h>
#include <IGUIStaticText.h>
#include <IGUICheckBox.h>
#include "windbot_panel.h"
#include <sstream>
#include <fmt/format.h>
#include "data_manager.h"

namespace ygo {

int WindBotPanel::CurrentIndex() {
	int selected = deckBox->getSelected();
	return selected >= 0 ? deckBox->getItemData(selected) : selected;
}

void WindBotPanel::Refresh(int filterMasterRule, int lastIndex) {
	int oldIndex = CurrentIndex();
	int lastBot = oldIndex >= 0 ? oldIndex : lastIndex;
	deckBox->clear();
	for (int i = 0; i < bots.size(); i++) {
		const auto& bot = bots[i];
		if (filterMasterRule == 0 || bot.masterRules.find(filterMasterRule) != bot.masterRules.end()) {
			int newIndex = deckBox->addItem(bot.name.c_str(), i);
			if (i == lastBot)
				deckBox->setSelected(newIndex);
		}
	}
	UpdateDescription();
}

void WindBotPanel::UpdateDescription() {
	int index = CurrentIndex();
	if (index < 0) {
		deckProperties->setText(L"");
		return;
	}
	auto& bot = bots[index];
	std::wstringstream params;
	if (bot.difficulty != 0)
		params << fmt::format(dataManager.GetSysString(2055), bot.difficulty);
	else
		params << dataManager.GetSysString(2056);
	params << L"\n";
	if (bot.masterRules.size()) {
		std::wstring mr; // Due to short string optimization, a stream is not needed
		for (auto rule : bot.masterRules) {
			if (mr.size())
				mr.append(L",");
			mr.append(std::to_wstring(rule));
		}
		params << fmt::format(dataManager.GetSysString(2057), mr.c_str());
		params << L"\n";
	}
	deckProperties->setText(params.str().c_str());
}

bool WindBotPanel::LaunchSelected(int port) {
	int index = CurrentIndex();
	if (index < 0) return false;
	// 1 = scissors, 2 = rock, 3 = paper
#if defined(_WIN32) || defined(__ANDROID__)
	return bots[index].Launch(port, !chkMute->isChecked(), chkThrowRock->isChecked() * 2);
#else
	auto pid = bots[index].Launch(port, !chkMute->isChecked(), chkThrowRock->isChecked() * 2);
	if(pid > 0) {
		windbotsPids.push_back(pid);
	}
	return pid > 0;
#endif
}

}