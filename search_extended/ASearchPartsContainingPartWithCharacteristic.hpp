/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/kpm/sc_agent.hpp"
#include "sc-memory/cpp/sc_memory.hpp"
#include "searchKeynodes.hpp"

#include "ASearchPartsContainingPartWithCharacteristic.generated.hpp"

namespace search_extended
{

class ASearchPartsContainingPartWithCharacteristic : public ScAgent
{
    SC_CLASS(Agent, Event(Keynodes::question_initiated, ScEvent::Type::AddOutputEdge))
    SC_GENERATED_BODY()

    SC_PROPERTY(Keynode("sc_agent_search_parts_containing_part_with_characteristic"), ForceCreate)
    static ScAddr msAgentKeynode;

    SC_PROPERTY(Keynode("question_search_parts_containing_part_with_characteristic"), ForceCreate)
    static ScAddr agent_question;

private:
    ScAddr class1;
    ScAddr class2;
    ScAddr nrel_characteristic;
    ScAddr value;
    sc_bool searchPartsClass1(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &answer, sc_bool &sys_off);
    sc_bool searchPartsClass2(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &answer, sc_bool &sys_off);
    sc_bool checkCharacteristicValue(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &answer, sc_bool &sys_off);

};

} // namespace
