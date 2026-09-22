#include "ramulator/controller/plugin/impl/command_counts.h"

#include "ramulator/controller/controller_base.h"
#include "ramulator/dram/dram_spec.h"

namespace Ramulator {

static std::vector<CommandCounts*>& instance_list() {
  static std::vector<CommandCounts*> list;
  return list;
}

const std::vector<CommandCounts*>& CommandCounts::instances() {
  return instance_list();
}

void CommandCounts::init() {
  instance_list().push_back(this);
  m_stats.add("num_opening_cmds", s_num_opening);
  m_stats.add("num_closing_bank_cmds", s_num_closing_bank);
  m_stats.add("num_closing_all_cmds", s_num_closing_all);
  m_stats.add("num_refreshing_cmds", s_num_refreshing);
}

void CommandCounts::setup(IFrontEnd* frontend, IMemorySystem* memory_system) {
  m_ctrl = cast_parent<ControllerBase>();
}

void CommandCounts::on_issue(const Request& req) {
  const auto& spec = *m_ctrl->m_device.m_spec;
  const auto& meta = spec.command_meta[req.command];
  if (meta.is_opening) {
    s_num_opening++;
  } else if (meta.is_closing) {
    if (spec.bank_targets[req.command] == BankTarget::Single) {
      s_num_closing_bank++;
    } else {
      s_num_closing_all++;
    }
  } else if (meta.is_refreshing) {
    s_num_refreshing++;
  }
}

void CommandCounts::reset_stats() {
  s_num_opening = 0;
  s_num_closing_bank = 0;
  s_num_closing_all = 0;
  s_num_refreshing = 0;
}

}  // namespace Ramulator
