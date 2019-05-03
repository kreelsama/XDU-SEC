	#define MAXSEQ 5
	
	mtype = {Msg,Ack,Nak,Err,Miss};
	chan SenderToReceiver = [1] of {mtype,byte,byte}	;
	chan ReceiverToSender = [1] of {mtype,byte,byte}	;
	
	proctype SENDER(chan InCh,OutCh)
	{
		byte SendData;
		byte SendSeq;
		byte ReceivedSeq;
		SendData = MAXSEQ-1;
		do
		   ::SendData = (SendData+1)%MAXSEQ;
	again: if
			::OutCh!Msg(SendData,SendSeq)
			::OutCh!Err(0,0)
			::OutCh!Miss(0,0)
		   fi;
	
		   if
			::timeout -> goto again
			::InCh?Miss(0,0) ->goto again
			::InCh?Err(0,0) ->goto again
			::InCh?Nak(ReceivedSeq,0) ->
	end1: 	goto again
			::InCh?Ack(ReceivedSeq,0) ->
				if
					::(ReceivedSeq == SendSeq) ->goto progress
					::(ReceivedSeq != SendSeq)  ->
	end2:			goto again
				fi
			fi;
	progress :	SendSeq = 1 - SendSeq;
		od;
	}
	
	proctype RECEIVER(chan InCh,OutCh)
	{
		byte ReceivedData;
		byte ReceivedSeq;
		byte ExpectedData;
		byte ExpectedSeq;
	
		do
		:: InCh?Msg(ReceivedData,ReceivedSeq) ->
			if 
			::(ReceivedSeq == ExpectedSeq) ->
				assert(ReceivedData == ExpectedData);
		progress: ExpectedSeq = 1- ExpectedSeq;
				ExpectedData = (ExpectedData + 1)%MAXSEQ;
				if
				::OutCh!Miss(0,0)
				::OutCh!Ack(ReceivedSeq,0);
				::OutCh!Err(0,0);
				ExpectedSeq = 1- ExpectedSeq;
				ExpectedData = (ExpectedData + 4)%MAXSEQ;
				fi
				::(ReceivedSeq!=ExpectedSeq) ->
				  if
				   ::OutCh!Nak(ReceivedSeq,0);
				   ::OutCh!Err(0,0);
				  fi
			fi
			::InCh?Err(0,0) -> OutCh!Nak(ReceivedSeq,0);
			::InCh?Miss(0,0) -> skip
		od;
	}
	
	init 
	{
		atomic
		{
			run SENDER(ReceiverToSender,SenderToReceiver);
			run RECEIVER(SenderToReceiver,ReceiverToSender);
		}
	}
