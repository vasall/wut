




# EVENT HANDLING

	   <start>
	      |
	      |
	      V
	SDL-EVENT-LOOP
	      |
	      | Spits out new event
	      V
	Collect Info (Window, Element)
	      |
	      | Throw at handling tree
	      V
	Process through all given functions
	      |
	      | If default handling is not disabled
	      V
	Process through all default handling functions
	      |
	      |
	      V
	    <end>
