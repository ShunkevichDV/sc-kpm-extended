/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/kpm/sc_agent.hpp"
#include "sc-memory/cpp/sc_memory.hpp"
#include "searchKeynodes.hpp"

#include "ASearchElementsConnectedWithRelation.generated.hpp"

namespace search_extended
{

class ASearchElementsConnectedWithRelation : public ScAgent
{
    SC_CLASS(Agent, Event(Keynodes::question_initiated, ScEvent::Type::AddOutputEdge))
    SC_GENERATED_BODY()

    SC_PROPERTY(Keynode("sc_agent_search_elements_connected_with_relation"), ForceCreate)
    static ScAddr msAgentKeynode;

    SC_PROPERTY(Keynode("question_search_elements_connected_with_relation"), ForceCreate)
    static ScAddr agent_question;

};

} // namespace bottle
