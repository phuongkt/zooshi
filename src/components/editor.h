// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COMPONENTS_EDITOR_H_
#define COMPONENTS_EDITOR_H_

#include <string>
#include <unordered_map>
#include <set>
#include "components_generated.h"
#include "entity/component.h"

namespace fpl {
namespace fpl_project {

struct EditorData {
  EditorData() : ignore_selection(-1) {}
  std::string entity_id;
  std::string prototype;
  std::string source_file;
  std::string comment;
  int ignore_selection;  // if > 0, skip this when cycling in the world editor
  // Keep track of which of this entity's components came from the prototype.
  std::set<ComponentDataUnion> components_from_prototype;
};

class EditorComponent : public entity::Component<EditorData> {
 public:
  virtual void AddFromRawData(entity::EntityRef& entity, const void* raw_data);
  void AddFromPrototypeData(entity::EntityRef& entity,
                            const EditorDef* editor_def);
  void AddWithSourceFile(entity::EntityRef& entity,
                         const std::string& source_file);
  virtual RawDataUniquePtr ExportRawData(entity::EntityRef& entity) const;
  virtual void* PopulateRawData(entity::EntityRef& entity, void* helper) const;

  virtual void InitEntity(entity::EntityRef& entity);
  virtual void CleanupEntity(entity::EntityRef& entity);
  virtual void UpdateAllEntities(entity::WorldTime /*delta_time*/) {}

  const std::string& GetEntityID(entity::EntityRef& entity);

  // Non-const because if we find an invalid entity, it gets silently removed.
  entity::EntityRef GetEntityFromDictionary(const std::string& key);

 private:
  void AddEntityToDictionary(const std::string& key, entity::EntityRef& entity);
  void RemoveEntityFromDictionary(const std::string& key);
  void GenerateRandomEntityID(std::string* output);

  std::unordered_map<std::string, entity::EntityRef> entity_dictionary_;
  std::string empty_string;
};

}  // namespace fpl_project
}  // namespace fpl

FPL_ENTITY_REGISTER_COMPONENT(fpl::fpl_project::EditorComponent,
                              fpl::fpl_project::EditorData,
                              ComponentDataUnion_EditorDef)

#endif  // COMPONENTS_EDITOR_H_
