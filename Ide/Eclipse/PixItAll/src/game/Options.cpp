#include "Options.h"
#include <Utils/Utils/Path.h>

#define TIXML_USE_STL
#include <Utils/Xml/tinyxml.h>

Options::Options()
{
	musicVol = 0.6f;
	soundsVol = 0.6f;
	showFinishedAllLevels = true;
	showAskForRate = true;
	askForRateAfterFinishedLevels = 5;
}

void Options::SetOptionsFile(const std::string &path)
{
	this->path = path;
}

bool Options::Load()
{
	assert(!path.empty());
	
	if (!Path::IsFileExists(path.c_str()))
		return false;
	
	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile(path))
		return false;
	
	const TiXmlElement *rootNode = xmlDoc.RootElement();
	if (rootNode == NULL || (strcmp(rootNode ->Value(), "options") != 0))
		return false;
	
	const TiXmlElement *rootElement = rootNode->ToElement();
	if (rootElement == NULL)
		return false;
		
	const TiXmlElement *el;
	
	int showFinished = 0;
	int showRate = 0;
	
	el = rootElement->FirstChildElement("music_volume");
	if (el != NULL && el->GetText() != NULL)
		sscanf(el->GetText(), "%f", &musicVol);
	
	el = rootElement->FirstChildElement("sound_volume");
	if (el != NULL && el->GetText() != NULL)
		sscanf(el->GetText(), "%f", &soundsVol);
		
		el = rootElement->FirstChildElement("show_finished_all_levels");
	if (el != NULL && el->GetText() != NULL)
		sscanf(el->GetText(), "%d", &showFinished);
	
	el = rootElement->FirstChildElement("ask_for_rate");
	if (el != NULL && el->GetText() != NULL)
		sscanf(el->GetText(), "%d", &showRate);
	
	el = rootElement->FirstChildElement("ask_for_rate_after_finished_levels");
	if (el != NULL && el->GetText() != NULL)
		sscanf(el->GetText(), "%d", &askForRateAfterFinishedLevels);

	showFinishedAllLevels = showFinished == 1;
	showAskForRate = showRate == 1;

	return true;
}

void Options::Save()
{
	assert(!path.empty());
	
	std::stringstream ss;
	ss << "<options>\n";
	ss << "\t<music_volume>" << musicVol << "</music_volume>\n";
	ss << "\t<sound_volume>" << soundsVol << "</sound_volume>\n";
	ss << "\t<show_finished_all_levels>" << showFinishedAllLevels << "</show_finished_all_levels>\n";
	ss << "\t<ask_for_rate>" << showAskForRate << "</ask_for_rate>\n";
	ss << "\t<ask_for_rate_after_finished_levels>" << askForRateAfterFinishedLevels << "</ask_for_rate_after_finished_levels>\n";
	ss << "</options>\n";
	
	std::ofstream file(path.c_str(), std::ios::trunc);
	if (file.fail())
		return;
	
	file.write(ss.str().c_str(), ss.str().length());
	file.close();
}

float Options::GetMusicVolume() const
{
	return musicVol;
}

void Options::SetMusicVolume(float volume)
{
	this->musicVol = volume;
}

float Options::GetSoundsVolume() const
{
	return soundsVol;
}

void Options::SetSoundsVolume(float volume)
{
	this->soundsVol = volume;
}

bool Options::GetShowFinishedAllLevels()
{
	return showFinishedAllLevels;
}

void Options::SetShowFinishedAllLevels(bool val)
{
	this->showFinishedAllLevels = val;
}

bool Options::GetAskForRate()
{
	return showAskForRate;
}

void Options::SetAskForRate(bool val)
{
	this->showAskForRate = val;
}

int Options::GetAskForRateAfterFinishedLevels() const
{
	return askForRateAfterFinishedLevels;
}
