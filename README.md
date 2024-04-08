# TCP_Srv
Yet another C++ TCP server implementation
## task
- implement a TCP server base class
- implement a sample derived class that echoes incoming data
- one implementation that builds with gcc / make and VS
## motivation
### see what AI can do
#### VS code with github copilot
I used the 30 days trial period to find out how effectively copilot enhances / speeds up development.
#### ChatGPT
Later in the development - just for curiosity - I asked ChatGPT for a solution.
### googeled solutions in the net
What I found googeling the issue wasn't really convincing.

for instance
- too complicated
- not suitable for both gcc and VS
### training
- modern C++
- python
- github CI
## results
### AI
#### github copilot
Github copilot made a mixed impression from extremely helpful to completely annoying.

Sometimes it really worked:
- just type a word - comment or code
- copilot proposes just what was intended or even better
- just a TAB and code is written

Sometimes it didn't
- proposals where completely unusable
- the occupation of TAB can confuse

Copilot worked as good with python as with C++.

Since it is AI - it sure will get better by the time.
#### ChatGPT
It took two questions to get a complete implementation for gcc, and a third one to get the VS version. Both versions were not 100 percent perfect but built and ran without modification.
- see [protocol of dialog](ChatGPT_dialog.md)

Moreover ChatGPT was extremely helpful with any other _how to_ or other question.

For instance I asked who has to resolve comments in a github peer review.

And what I got was substantial explanation of the process.

#### conclusion
Once one gets used to it github copilot enhances the development to a certain degree.

ChatGPT is a boost. It will make google and sites like StackOverflow more or less redundant.

(The price of this is a consumption of electric energy equivalent to a town with 150000 - yes: a hundred and fifty thousand - inhabitants.)

Anyhow: AI has changed the approach to software development
- less work with implementation issues
- more concentration on issues like
    - subject
    - requirements
    - collaboration

## docs
- [different server solutions](solutions.md)
- [how to build](make/howto_build.md)
- [imlementations folder](code)

