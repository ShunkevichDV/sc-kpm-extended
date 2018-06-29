/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/kpm/sc_agent.hpp"
#include "sc-memory/cpp/sc_memory.hpp"
#include "searchKeynodes.hpp"

#include "ASearchOutsidePart.generated.hpp"

namespace search_extended
{

class ASearchOutsidePart : public ScAgent
{
    SC_CLASS(Agent, Event(Keynodes::question_initiated, ScEvent::Type::AddOutputEdge))
    SC_GENERATED_BODY()

    SC_PROPERTY(Keynode("sc_agent_search_outside_part"), ForceCreate)
    static ScAddr msAgentKeynode;

    SC_PROPERTY(Keynode("question_search_outside_part"), ForceCreate)
    static ScAddr agent_question;

    SC_PROPERTY(Keynode("nrel_external_to_internal_clamping"), ForceCreate)
    static ScAddr nrel_external_to_internal_clamping;

};

} // namespace bottle
