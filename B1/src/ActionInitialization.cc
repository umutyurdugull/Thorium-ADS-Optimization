#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

namespace B1
{

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);

  auto runAction = new RunAction;
  SetUserAction(runAction);

  auto eventAction = new EventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new SteppingAction(eventAction));
}

}

/* NOTES:
1. Removed empty constructor/destructor definitions because they are already handled by '= default' in the header.
2. EventAction now correctly receives a pointer to RunAction as required by its constructor.
3. SteppingAction now correctly receives a pointer to EventAction to track step-level data during events.
4. This structure ensures thread-safety by creating local instances of actions for each worker thread.
*/