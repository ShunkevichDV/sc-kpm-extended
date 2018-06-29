/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "search_extended.hpp"

#include "ASearchPartsOfGivenClasses.hpp"
#include "ASearchPartsByRelations.hpp"
#include "ASearchElementsConnectedWithRelation.hpp"
#include "ASearchOutsidePart.hpp"
#include "searchKeynodes.hpp"

#include <iostream>

using namespace search_extended;

SC_IMPLEMENT_MODULE(searchExtendedModule)

sc_result searchExtendedModule::InitializeImpl()
{
    if (!Keynodes::InitGlobal())
        return SC_RESULT_ERROR;

    SC_AGENT_REGISTER(ASearchPartsOfGivenClasses)
    SC_AGENT_REGISTER(ASearchPartsByRelations)
    SC_AGENT_REGISTER(ASearchElementsConnectedWithRelation)
    SC_AGENT_REGISTER(ASearchOutsidePart)


    return SC_RESULT_OK;
}

sc_result searchExtendedModule::ShutdownImpl()
{
    SC_AGENT_UNREGISTER(ASearchPartsOfGivenClasses)
    SC_AGENT_UNREGISTER(ASearchPartsByRelations)
    SC_AGENT_UNREGISTER(ASearchElementsConnectedWithRelation)
    SC_AGENT_UNREGISTER(ASearchOutsidePart)

    sc_result result = SC_RESULT_OK;
    return result;
}
