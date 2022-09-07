InvitedPlayersFile = "C://Users//Acrozo//Documents//Xenobot//Players.txt"


InviteMessages = {"inv", "invite", "play"}
LeaveMessages = {"Goodbye", "bye", "cya"}
GamblingMessages = {"h", "l", "high", "low", "payout", "info", "balance"}


CharacterName = "Sylluss"
MaxIdleTime = 35 -- seconds 
SQMRange = 5 -- squares away

CurrentPlayer = "" -- leave blank

function InvitePlayer(text)
  local file = io.open(InvitedPlayersFile, "w+")
  if file ~= nil then
    file:write(CurrentPlayer)
    file:close()
  end
end

function KickPlayer()
  Self.Say("Goodbye ".. CurrentPlayer.."!")
  CurrentPlayer = ""
  InvitePlayer("")
end

function inTable(tbl, item)
  for key, value in pairs(tbl) do
    if value == item then
      return key
    end
  end
  return false
end

local PlayerGreeting = Module.New("PlayerGreeting", function(mod)
  Self.Say("Greetings "..CurrentPlayer.." !")
  mod:Stop()
end, false)

idleTime = 0
Module.New("timer", function(module)
  if CurrentPlayer == "" then
    idleTime = 0
  else
    idleTime = idleTime + 1
  end
  if idleTime > MaxIdleTime and CurrentPlayer ~= "" then
    KickPlayer()
  end
  module:Delay(1000)
end)


Module.New("Check_For_Players_OnScreen", function(module)
  local verify = 0
  for _, c in Creature.iPlayers(SQMRange) do
    if c:Name() == CurrentPlayer then
      verify = verify + 1
      break
    end
  end
  if verify == 0 and CurrentPlayer ~= "" then
    KickPlayer()
  end
  module:Delay(2000)
end)

InvitePlayer("")

function localProxy(proxy, msgType, speaker, lvl, text)
  if msgType == "say" and speaker ~= Self.Name() and inTable(InviteMessages, text:lower()) ~= false then
    CurrentPlayer = speaker
    InvitePlayer(CurrentPlayer)
    PlayerGreeting:Start()
  end
  text:lower()
  if CurrentPlayer == speaker and inTable(GamblingMessages, text:lower()) then
    idleTime = 0
  end
  if CurrentPlayer == speaker and inTable(LeaveMessages, text:lower()) then
    KickPlayer()
  end
end

LocalSpeechProxy.OnReceive("Local Message Proxy", localProxy)