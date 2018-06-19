/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "search_extended.hpp"

#include "ASearchPartsOfGivenClasses.hpp"

using namespace search_extended;

SC_IMPLEMENT_MODULE(searchExtendedModule)

sc_result searchExtendedModule::InitializeImpl()
{

    SC_AGENT_REGISTER(ASearchPartsOfGivenClasses)

    return SC_RESULT_OK;
}

sc_result searchExtendedModule::ShutdownImpl()
{
    SC_AGENT_UNREGISTER(ASearchPartsOfGivenClasses)

    sc_result result = SC_RESULT_OK;
    return result;
}
