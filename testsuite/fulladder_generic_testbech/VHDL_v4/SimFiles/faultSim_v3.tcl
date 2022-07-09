
when -label faultInjectionwhen " faultInjection == '1' and faultInjection'event " { 
	variable faultLoc [exa sim:/fulladder_TB/FI/wireName]
	variable faultLoc [string trim $faultLoc "{}"]
	variable faultVal [exa sim:/fulladder_TB/FI/stuckAtVal]
	variable faultVal [string trim $faultVal "{}"]
	
	force -freeze sim:$faultLoc $faultVal
#	echo "tcl---->fault $numOfFaults is injected @ $now"
}
when -label faultRemovalwhen " faultInjection == '0' and faultInjection'event " { 
	variable faultLoc [exa sim:/fulladder_TB/FI/wireName]
	variable faultLoc [string trim $faultLoc "{}"]
	noforce sim:$faultLoc
#	echo "tcl---->fault $numOfFaults is removed @ $now"
}



