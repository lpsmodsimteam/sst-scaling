// Created for SST-Core Version (9.1.0)
//
#include "component_switch.h"
#include "ExampleEvent.h"
#include <iostream>
// to hold a list of ports
#include <vector>


using namespace MultipleCopies; // defined in the .h which was included above

// Component constructor.
// using class from the .h included above
SwitchComponent::SwitchComponent(SST::ComponentId_t id, SST::Params &params) :
    SST::Component(id),
    componentId_(id),
    clockTickCount_(0)
{
    // Read in the parameters from the python config file.  See SST_ELI_DOCUMENT_PARAMS
    // for an explanation of what each parameter represents.
    //
    std::string clock =
        params.find<std::string>("clock", "1GHz");
    clockTicks_ = static_cast<uint64_t>(
        params.find<int>("clockTicks", 10));
    unsigned int debug =
        params.find<int>("debug", ALL);

    // Create the logger.
    //
    logger_ = SST::Output("Time=@t; File=@f; Func=@p; Line=@l; Thread=@I -- ", debug, 0x01, SST::Output::STDOUT);
    logger_.verbose(CALL_INFO, TRACE, 0x00, "Entering constructor for component id %lu\n", componentId_);

    // Initialize the debug output instance.
    // Strings for debug output use the printf format.
    //
    logger_.verbose(CALL_INFO, INFO,  0x00, "Initializing component %lu.\n", id);
    logger_.verbose(CALL_INFO, DEBUG, 0x00, "Parameters successfully read from config file.\n");
    logger_.verbose(CALL_INFO, DEBUG, 0x00, "clockTicks = %lu\n", clockTicks_);
    logger_.verbose(CALL_INFO, INFO,  0x00, "Constructing new Example Instance.\n");

    // Configure the links (connections to other components).
    // The link is associated with a component's registered port.
    // No handler for this example.
    //
    logger_.verbose(CALL_INFO, DEBUG, 0x00, "Configuring link.\n");
    // port_a = configureLink("port_a");
    // port_b = configureLink("port_b");
    std::vector<SST::Link*> ports;

    // from https://chryswoods.com/beginning_c++/lists.html
    for (int index=0; index<=5; ++index)
    {
        ports.push_back( configureLink("port_"+std::to_string(index)) );
    }
   port_0 = configureLink("port_0");
   port_1 = configureLink("port_1");
   port_2 = configureLink("port_2");
   port_3 = configureLink("port_3");
   port_4 = configureLink("port_4");
   port_5 = configureLink("port_5");
   port_6 = configureLink("port_6");

// how to access what is in the vector:
//for (int i=0; i<v.size(); ++i){
//    std::cout << v[i] << " ";
//}


    // Configure the component clock.
    //
    logger_.verbose(CALL_INFO, DEBUG, 0x00, "Clock rate is: %s\n", clock.c_str());
    registerClock(clock,
        new SST::Clock::Handler<SwitchComponent>(this, &SwitchComponent::clockTick));
    logger_.verbose(CALL_INFO, INFO,  0x00, "Successfully initialized clock.\n");

    // Register this component with the simulation.
    //
    registerAsPrimaryComponent();
    logger_.verbose(CALL_INFO, DEBUG, 0x00, "Component registered as primary component.\n");
    primaryComponentDoNotEndSim();
    logger_.verbose(CALL_INFO, DEBUG, 0x00, "Simulation notified it should not end.\n");
    logger_.verbose(CALL_INFO, TRACE, 0x00, "Leaving constructor for component id %lu\n", componentId_);
}


// Called after all components have been constructed and initialization
// has completed, but before simulation time has begin.
//
// This is where you should do any other initialization that needs to be done
// but could be accomplished in the constructure.
//
void SwitchComponent::setup(void)
{
    logger_.verbose(CALL_INFO, TRACE, 0x00, "Entering setup for component id %lu\n", componentId_);
    logger_.verbose(CALL_INFO, TRACE, 0x00, "Leaving setup for component id %lu\n", componentId_);
}


// Called after the simulation is complete but before the objects are
// destroyed.  This is a good place to print out statistics.
//
void SwitchComponent::finish(void)
{
    logger_.verbose(CALL_INFO, TRACE, 0x00, "Entering finish for component id %lu\n", componentId_);
    logger_.verbose(CALL_INFO, TRACE, 0x00, "Leaving finish for component id %lu\n", componentId_);
}


// Clock event handler.
//
bool SwitchComponent::clockTick(SST::Cycle_t cycle)
{
    logger_.verbose(CALL_INFO, TRACE, 0x00, "Entering clock for component id %lu\n", componentId_);
    bool done = false;

    // Poll the link for incoming messages and process them as necessary.
    //
    logger_.verbose(CALL_INFO, DEBUG, 0x00, "Retreiving event from link in component id %lu\n", componentId_);
    ExampleEvent* ev = static_cast<ExampleEvent*>(port_0->recv());

    // nullptr is a keyword that can be used at all places where NULL is expected.
    if (nullptr == ev)
    {
        logger_.verbose(CALL_INFO, DEBUG, 0x00, "No event available from link in component id %lu\n", componentId_);
    }
    else
    {
        // Increment the clock tick counter and end when you get to
        // the specified value.
        //
        clockTickCount_+= 1;
        done = (clockTickCount_ == clockTicks_);
        logger_.verbose(CALL_INFO, INFO, 0x00, "Clock tick count = %lu out of %lu\n", clockTickCount_, clockTicks_);
        if (done)
        {
            logger_.verbose(CALL_INFO, INFO, 0x00, "Ending sim.\n");
            primaryComponentOKToEndSim();
        }
    }

    // Send an event over the link.
    //
    logger_.verbose(CALL_INFO, INFO, 0x00, "Sending event over the link.\n");
    port_0->send(ev);

    logger_.verbose(CALL_INFO, TRACE, 0x00, "Leaving clock for component id %lu\n", componentId_);
    return done;
}
