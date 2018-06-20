/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/sc_memory.h"
#include "sc-memory/cpp/sc_module.hpp"

#include "searchKeynodes.generated.hpp"

namespace search_extended
{

class Keynodes : public ScObject
{
    SC_CLASS()
    SC_GENERATED_BODY()

public:

    SC_PROPERTY(Keynode("question_initiated"), ForceCreate)
    static ScAddr question_initiated;

    SC_PROPERTY(Keynode("nrel_inclusion"), ForceCreate)
    static ScAddr nrel_inclusion;

    SC_PROPERTY(Keynode("nrel_subdividing"), ForceCreate)
    static ScAddr nrel_subdividing;

    SC_PROPERTY(Keynode("system_element"), ForceCreate)
    static ScAddr system_element;

    SC_PROPERTY(Keynode("rrel_1"), ForceCreate)
    static ScAddr rrel_1;

    SC_PROPERTY(Keynode("rrel_2"), ForceCreate)
    static ScAddr rrel_2;
};

} //namespace
