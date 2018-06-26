/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/kpm/sc_agent.hpp"
#include "sc-memory/cpp/sc_memory.hpp"
#include "searchKeynodes.hpp"

#include "ASearchPartsByRelations.generated.hpp"

namespace search_extended
{

class ASearchPartsByRelations : public ScAgent
{
    SC_CLASS(Agent, Event(Keynodes::question_initiated, ScEvent::Type::AddOutputEdge))
    SC_GENERATED_BODY()

    SC_PROPERTY(Keynode("sc_agent_search_parts_by_relations"), ForceCreate)
    static ScAddr msAgentKeynode;

    SC_PROPERTY(Keynode("question_search_all_parts_by_given_relations"), ForceCreate)
    static ScAddr agent_question;

    SC_PROPERTY(Keynode("question_count_all_parts_by_given_relations"), ForceCreate)
    static ScAddr agent_question_count;

private:
    int count;
    sc_bool searchByRelationsRec(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &class_set, ScAddr const &answer, sc_bool &sys_off, sc_bool &count_only);

};

} // namespace bottle
