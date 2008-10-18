#include "levelset.h"
#include "level.h"
#include "textsections.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>

static std::vector<std::string> directoryEntries(const std::string & path) {

  DIR * dir = ::opendir(path.c_str());
  if (dir == NULL)
    throw format_error("unable to open directory: " + path);
  std::vector<std::string> entries;
  for (struct dirent * i = ::readdir(dir); i != NULL; i = ::readdir(dir)) {
    entries.push_back(i->d_name);
  }
  if (::closedir(dir) != 0)
    throw format_error("unable to close directory: " + path);
  return entries;
}

levelset_c::levelset_c(const std::string & path) {

  const std::vector<std::string> entries = directoryEntries(path);
  level_c test_level;
  bool index_loaded = false;
  for (std::vector<std::string>::const_iterator i = entries.begin(); i != entries.end(); i++) {

    const std::string & filename = *i;
    if (filename.size() <= 0 || filename[0] == '.')
      continue;

    std::ifstream file((path + '/' + filename).c_str());
    const textsections_c sections(file, true);
    try {

      /* Load level */
      test_level.load(sections);

    } catch (format_error & level_e) {

      /* Load levelset index */
      if (index_loaded)
        throw;
      try {

        /* Version section */
        {
          std::istringstream versionStream(sections.getSingleLine("Version"));
          unsigned int givenVersion;
          versionStream >> givenVersion;
          if (!versionStream.eof() || !versionStream)
            throw format_error("invalid levelset index version");
        }

        /* Name section */
        name = sections.getSingleLine("Name");

        /* Priority section */
        {
          std::istringstream priorityStream(sections.getSingleLine("Priority"));
          priorityStream >> priority;
          if (!priorityStream.eof() || !priorityStream)
            throw format_error("invalid levelset priority number");
        }

        /* Levels section */
        levelNames = sections.getSingleSection("Levels");

        index_loaded = true;

      } catch (format_error & index_e) {
        throw format_error("file " + filename + " is"
                           " neither a level (" + level_e.msg + ")"
                           " nor a levelset index (" + index_e.msg + ")");
      }
      continue;
    }
    const std::string & levelName = test_level.getName();
    if (!levels.insert(make_pair(levelName, sections)).second)
      throw format_error("duplicate level name: " + levelName);
  }

  /* Check for consistency and completeness */
  if (!index_loaded)
    throw format_error("missing levelset index");
  std::map<std::string, bool> levelNamesMap;
  for (std::vector<std::string>::const_iterator i = levelNames.begin(); i != levelNames.end(); i++) {
    const std::string & levelName = *i;
    if (levels.find(levelName) == levels.end())
      throw format_error("missing level: " + levelName);
    if (!levelNamesMap.insert(make_pair(levelName, true)).second)
      throw format_error("duplicate entry in levelset index for level: " + levelName);
  }
  for (std::map<std::string, textsections_c>::const_iterator i = levels.begin(); i != levels.end(); i++) {
    const std::string & levelName = i->first;
    if (levelNamesMap.find(levelName) == levelNamesMap.end())
      throw format_error("missing entry in levelset index for level: " + levelName);
  }
}

void levelset_c::loadLevel(level_c & level, const std::string & levelName) const {

  const std::map<std::string, textsections_c>::const_iterator i = levels.find(levelName);
  if (i == levels.end())
    throw format_error("unknown level name: " + levelName);
  level.load(i->second);
}

void levelsetList_c::load(const std::string & path) {

  /* load new levelsets */
  const std::vector<std::string> entries = directoryEntries(path);
  for (std::vector<std::string>::const_iterator i = entries.begin(); i != entries.end(); i++) {

    const std::string & entryname = *i;
    if (entryname.size() <= 0 || entryname[0] == '.')
      continue;

    levelset_c levelset(path + '/' + entryname);
    const std::string & levelsetName = levelset.getName();
    if (!levelsets.insert(make_pair(levelsetName, levelset)).second)
      throw format_error("duplicate levelset name: " + levelsetName);
    sortHelper.push_back(make_pair(levelset.getPriority(), levelsetName));
  }

  /* re-sort all levelsets by priority and name */
  std::sort(sortHelper.begin(), sortHelper.end());
  levelsetNames.clear();
  for (std::vector<std::pair<unsigned int, std::string> >::const_iterator i = sortHelper.begin(); i != sortHelper.end(); i++)
    levelsetNames.push_back(i->second);
}

const levelset_c & levelsetList_c::getLevelset(const std::string & levelsetName) const {

  const std::map<std::string, levelset_c>::const_iterator i = levelsets.find(levelsetName);
  if (i == levelsets.end())
    throw format_error("unknown levelset name: " + levelsetName);
  return i->second;
}
