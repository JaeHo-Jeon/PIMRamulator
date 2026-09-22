#ifndef RAMULATOR_CONTROLLER_PLUGIN_COMMAND_COUNTS_H
#define RAMULATOR_CONTROLLER_PLUGIN_COMMAND_COUNTS_H

#include <cstdint>
#include <vector>

#include "ramulator/base/base.h"
#include "ramulator/controller/plugin/i_controller_plugin.h"

namespace Ramulator {

class ControllerBase;

/// Counts the DRAM commands a controller issues, by what they do to the bank:
/// open a row, close one bank's row, close every bank's, or refresh. Standard
/// independent, so an ACT and LPDDR5's ACT1 both count as an opening.
///
/// The counts are statistics, so they appear in print_stats(); they can also
/// be read in process, by whoever built the memory system, from instances().
///
/// Example config (Python):
///   ramulator.controller_plugin.CommandCounts()
class CommandCounts final : public IControllerPlugin, public Implementation {
  RAMULATOR_REGISTER_IMPLEMENTATION(IControllerPlugin, CommandCounts, "CommandCounts")

 public:
  uint64_t s_num_opening = 0;       // ACT, ACT1
  uint64_t s_num_closing_bank = 0;  // PREpb, PRE
  uint64_t s_num_closing_all = 0;   // PREab, PREA, PREsb
  uint64_t s_num_refreshing = 0;    // REFab, REFpb, REFsb

  /// Every instance built so far, in construction order. A frontend that owns
  /// one memory system notes the size before Factory::create_memory_system and
  /// reads the instances added after it.
  static const std::vector<CommandCounts*>& instances();

  void init() override;
  void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override;
  void on_issue(const Request& req) override;
  void reset_stats() override;

 private:
  ControllerBase* m_ctrl = nullptr;
};

}  // namespace Ramulator

#endif  // RAMULATOR_CONTROLLER_PLUGIN_COMMAND_COUNTS_H
