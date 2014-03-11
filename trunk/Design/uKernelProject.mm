<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1372522461542" ID="ID_347795486" MODIFIED="1383511647400" STYLE="bubble" TEXT="uKernelProject">
<node CREATED="1372522602697" HGAP="117" ID="ID_1207419212" MODIFIED="1379869276255" POSITION="right" STYLE="bubble" TEXT="Design/Implementation Steps" VGAP="0" VSHIFT="-45">
<node CREATED="1372522683296" HGAP="347" ID="ID_461742411" MODIFIED="1378249649611" STYLE="bubble" TEXT="Scheduling" VSHIFT="-54">
<node CREATED="1377950793509" HGAP="96" ID="ID_789930452" MODIFIED="1379171990373" STYLE="bubble" TEXT="ContextSwitch" VSHIFT="-84">
<node CREATED="1377719008269" HGAP="95" ID="ID_192868394" MODIFIED="1379171988527" STYLE="bubble" TEXT="Steps" VSHIFT="-170">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      -) Locate current TCB
    </p>
    <p>
      -) Get stack pointer
    </p>
    <p>
      -) save current context to stack
    </p>
    <p>
      -) save new Top Of Stack in the TCB
    </p>
    <p>
      -) find highest priority tcb
    </p>
    <p>
      -) get stack pointer
    </p>
    <p>
      -) restore context from stack
    </p>
    <p>
      -) restore PC
    </p>
  </body>
</html></richcontent>
<cloud/>
<node CREATED="1377950422444" HGAP="64" ID="ID_797812348" MODIFIED="1378248784709" STYLE="bubble" TEXT="LocateCurrentTCB" VSHIFT="-53">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      use a global variable that holds the current tcb, all the time.
    </p>
  </body>
</html></richcontent>
</node>
<node CREATED="1377950444091" HGAP="69" ID="ID_91218468" MODIFIED="1378248784709" STYLE="bubble" TEXT="Save currentContext to stack" VSHIFT="-32"/>
<node CREATED="1377950464716" HGAP="123" ID="ID_1728306286" MODIFIED="1378249772319" STYLE="bubble" TEXT="GetStackPointer of high task" VSHIFT="-5"/>
<node CREATED="1378249699105" HGAP="127" ID="ID_363224412" MODIFIED="1378249777757" TEXT="Restore Context" VSHIFT="21"/>
<node CREATED="1378249754801" HGAP="103" ID="ID_83323561" MODIFIED="1378249775656" TEXT="continue execution of new task" VSHIFT="30"/>
</node>
<node CREATED="1379171990374" ID="ID_1760464217" MODIFIED="1379172004596" TEXT="ways a task can be switched">
<node CREATED="1379172017044" HGAP="38" ID="ID_469527445" MODIFIED="1379172136051" TEXT="Task calls yield" VSHIFT="-80"/>
<node CREATED="1379172018381" ID="ID_472160176" MODIFIED="1379172112795" TEXT="Message sent to a higher priority pending task">
<node CREATED="1379172118327" ID="ID_1531550190" MODIFIED="1379172124572" TEXT="Message sent from interrupt"/>
<node CREATED="1379172126606" ID="ID_1211598294" MODIFIED="1379172131583" TEXT="message sent from task it self"/>
</node>
<node CREATED="1379172055686" ID="ID_1825784467" MODIFIED="1379172059630" TEXT="Interrupts">
<node CREATED="1379172005833" HGAP="33" ID="ID_872669877" MODIFIED="1379172067647" TEXT="Interrupt from a device enables higher priority task" VSHIFT="-40"/>
<node CREATED="1379172015837" ID="ID_187865900" MODIFIED="1379172053916" TEXT="Interrupt from timer enables a higher priority task"/>
</node>
</node>
</node>
<node CREATED="1377950909826" HGAP="96" ID="ID_150363064" MODIFIED="1378249786140" STYLE="bubble" TEXT="FindHighestPriorityTask" VSHIFT="25"/>
</node>
<node CREATED="1372522687674" HGAP="352" ID="ID_1791110870" MODIFIED="1378248952844" STYLE="bubble" TEXT="Services" VSHIFT="-72">
<node CREATED="1378249833668" HGAP="71" ID="ID_1153166873" MODIFIED="1378249848268" TEXT="InterTask Communication" VSHIFT="-10">
<node CREATED="1380572688359" HGAP="51" ID="ID_96994051" MODIFIED="1380572699006" TEXT="message queues" VSHIFT="10">
<node CREATED="1382817876095" ID="ID_1840691217" MODIFIED="1382817882640" TEXT="so we have">
<node CREATED="1382817895967" ID="ID_1594511166" MODIFIED="1382817901470" TEXT="the actual message queues">
<node CREATED="1382817932191" ID="ID_439153142" MODIFIED="1382817937918" TEXT="takes void * elements"/>
<node CREATED="1382817944089" ID="ID_104907199" MODIFIED="1382817952024" TEXT="has max length"/>
<node CREATED="1382817953819" ID="ID_42690333" MODIFIED="1382817966071" TEXT="statically allocated size"/>
</node>
<node CREATED="1382817904210" ID="ID_1806885185" MODIFIED="1382817928774" TEXT="the task wait queues (actually lists in our case or priority queues)">
<node CREATED="1382817968272" ID="ID_1449145725" MODIFIED="1382817973530" TEXT="takes task * elements"/>
<node CREATED="1382817975338" ID="ID_1211822022" MODIFIED="1382817983107" TEXT="no need to have max length I thing"/>
<node CREATED="1382817984901" ID="ID_1669251681" MODIFIED="1382817992374" TEXT="dynamically allocated, "/>
</node>
</node>
</node>
</node>
<node CREATED="1378249853927" HGAP="82" ID="ID_488750941" MODIFIED="1378249864873" TEXT="Time Management" VSHIFT="2"/>
<node CREATED="1378249878119" HGAP="80" ID="ID_1374096144" MODIFIED="1380572686788" TEXT="Synchronization" VSHIFT="21">
<node CREATED="1380572677406" ID="ID_1833257302" MODIFIED="1380572680249" TEXT="semaphores"/>
<node CREATED="1380572681920" ID="ID_1780383056" MODIFIED="1380572683995" TEXT="mutexes"/>
</node>
</node>
<node CREATED="1372522792922" HGAP="361" ID="ID_541413655" MODIFIED="1378249632567" STYLE="bubble" TEXT="TimerInterrupt" VSHIFT="-40"/>
<node CREATED="1378248847208" HGAP="323" ID="ID_383376608" MODIFIED="1380575436988" TEXT="TaskManagement" VSHIFT="48">
<node CREATED="1372522677551" HGAP="77" ID="ID_1975668259" MODIFIED="1378249639620" STYLE="bubble" TEXT="TaskCreate" VGAP="22" VSHIFT="28">
<node CREATED="1372522891110" HGAP="28" ID="ID_566033240" MODIFIED="1378248784707" STYLE="bubble" TEXT="Register" VSHIFT="3">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      - find location of current tcb
    </p>
    <p>
      - get stack pointer
    </p>
    <p>
      - save unsaved context (registers)
    </p>
    <p>
      -
    </p>
  </body>
</html></richcontent>
</node>
<node CREATED="1372522887569" ID="ID_1657741029" MODIFIED="1378248784707" STYLE="bubble" TEXT="TaskControlBlock">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      typedef struct task_struct {
    </p>
    <p>
      uint_16 prio;&#160;&#160;&#160;&#160;&#160;&#160;&#160;/* Priority of corresponding task */
    </p>
    <p>
      uint_8 state; /* State of task */
    </p>
    <p>
      uint_32 tos; /* Top of task's stack */
    </p>
    <p>
      uint_32 *stack;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;/* */
    </p>
    <p>
      } TCB;
    </p>
  </body>
</html></richcontent>
<node CREATED="1372523261846" ID="ID_1638898274" MODIFIED="1378248784707" STYLE="bubble" TEXT="TCBAlloc">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      This one Allocates memory for the TaskControlBlock. Also Calls the StackAlloc function for stack allocation
    </p>
  </body>
</html></richcontent>
</node>
<node CREATED="1372523266173" ID="ID_500519502" MODIFIED="1378248784708" STYLE="bubble" TEXT="TCBInit">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      Setup the TCB fields with the initial state of the task.
    </p>
  </body>
</html></richcontent>
</node>
<node CREATED="1372523267402" ID="ID_1365610249" MODIFIED="1378248784708" STYLE="bubble" TEXT="TCBLink"/>
</node>
<node CREATED="1372522890376" ID="ID_1508680622" MODIFIED="1378248784708" STYLE="bubble" TEXT="TaskStack">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      The stack of an preempted/interrupted task must contain the follwoing information:
    </p>
    <p>
      - Register map of the interrupted/preempted task's execution context
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;- includes PC, SP, LR, GPRs, Status Reg
    </p>
    <p>
      -
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node CREATED="1380575437950" ID="ID_1902677716" MODIFIED="1380575442481" TEXT="priority inheritance"/>
<node CREATED="1380575445382" ID="ID_1830014193" MODIFIED="1380575450675" TEXT="priority ceiling protocol "/>
<node CREATED="1380575452569" ID="ID_1684182669" MODIFIED="1380575453421" TEXT="etc"/>
</node>
<node CREATED="1378249948307" HGAP="207" ID="ID_1532337230" MODIFIED="1378333172835" TEXT="Interrupt Handling" VSHIFT="-90"/>
<node CREATED="1378333178469" HGAP="261" ID="ID_1044491447" MODIFIED="1380410156246" TEXT="Exception Handling" VSHIFT="-173">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      Handle all exceptions!
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node COLOR="#006633" CREATED="1380410134518" HGAP="116" ID="ID_1164481672" MODIFIED="1387121469339" POSITION="left" TEXT="current- now active" VSHIFT="-214">
<node CREATED="1380410163386" ID="ID_1088921537" MODIFIED="1380410269212">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      Cleanup code and write comments
    </p>
  </body>
</html></richcontent>
</node>
<node CREATED="1380410273454" ID="ID_1695672094" MODIFIED="1380410296169" TEXT="implement alternatives for contex switch and for find highest prio task and measure"/>
<node CREATED="1381696336102" ID="ID_1898839791" MODIFIED="1381696350192" TEXT="need for a module that handles synchronization">
<node CREATED="1381696357119" ID="ID_281787019" MODIFIED="1381696359419" TEXT="mutexes"/>
<node CREATED="1381696361070" ID="ID_216515602" MODIFIED="1381696370991" TEXT="priority inheritance, ceiling etc."/>
</node>
<node CREATED="1382819221837" HGAP="244" ID="ID_320774760" MODIFIED="1386623325228" TEXT="CHANGE THE CRITICAL SECTION MECHANISM" VSHIFT="32">
<node CREATED="1382819246441" ID="ID_1717713970" MODIFIED="1382819294033" TEXT="FreeRTOS lets you switch from withing critical section, because it stores and restores the basepri register from within PendSVHandler"/>
<node CREATED="1382819296357" ID="ID_621726196" MODIFIED="1382903688941" TEXT="we don&apos;t allow switching from within critical sections and that&apos;s baaad for rtos"/>
</node>
<node CREATED="1382906765820" HGAP="206" ID="ID_36651974" MODIFIED="1386623306388" TEXT="change ReadyTaskBitmap and ReadyTaskIndexto uint_8 and use a LUT to find the hihgest prio resady task" VSHIFT="28"/>
<node CREATED="1383247684551" HGAP="205" ID="ID_252924818" MODIFIED="1386623294946" TEXT="ldex/stex -- see 34.3.2.7 Synchronization primitives" VSHIFT="8"/>
<node CREATED="1383326432017" HGAP="65" ID="ID_147303228" MODIFIED="1386623291558" TEXT="maybe change all tasks linked list to contain only the delayed tasks" VSHIFT="14">
<icon BUILTIN="yes"/>
<icon BUILTIN="help"/>
</node>
<node CREATED="1383775522780" FOLDED="true" ID="ID_770168667" MODIFIED="1386623458444" TEXT="memory management (heap) implementation has two problems">
<icon BUILTIN="yes"/>
<icon BUILTIN="yes"/>
<icon BUILTIN="yes"/>
<icon BUILTIN="yes"/>
<icon BUILTIN="yes"/>
<icon BUILTIN="yes"/>
<icon BUILTIN="yes"/>
<icon BUILTIN="yes"/>
<icon BUILTIN="yes"/>
<node CREATED="1386623226664" ID="ID_993612955" MODIFIED="1386623259454" TEXT="1. Force alignment. union is added in heap definition but portMalloc should be cahgned accordingly"/>
<node CREATED="1386623261211" ID="ID_822664800" MODIFIED="1386623286715" TEXT="It crashes on Keil. Find out why.. Heap.c should be the same on both compilers"/>
</node>
<node CREATED="1387121465300" ID="ID_542060630" MODIFIED="1387121466821" TEXT="shell"/>
<node CREATED="1387121472035" ID="ID_514023346" MODIFIED="1387121475804" TEXT="fs"/>
<node CREATED="1387121478538" ID="ID_1167781327" MODIFIED="1387121485742" TEXT="round robin for same prio tasks"/>
</node>
<node CREATED="1386623371851" HGAP="313" ID="ID_1848788446" MODIFIED="1386623384694" POSITION="left" TEXT="Done Issues" VSHIFT="-107">
<node CREATED="1381963135505" HGAP="-40" ID="ID_1707777245" MODIFIED="1386623334847" TEXT="interrupt enter-exit" VSHIFT="20">
<node CREATED="1382041291627" ID="ID_328074029" MODIFIED="1382903700691" TEXT="Use OSIntEnter and OSIntExit on every interrupt handler is one solution">
<icon BUILTIN="button_ok"/>
</node>
</node>
<node CREATED="1380572688359" HGAP="51" ID="ID_110759574" MODIFIED="1380572699006" TEXT="message queues" VSHIFT="10">
<node CREATED="1382817876095" ID="ID_1042635670" MODIFIED="1387127507094" TEXT="d">
<node CREATED="1382817895967" ID="ID_930817037" MODIFIED="1383424330626" TEXT="the actual message queues">
<icon BUILTIN="button_ok"/>
<node CREATED="1382817932191" ID="ID_749612586" MODIFIED="1382817937918" TEXT="takes void * elements"/>
<node CREATED="1382817944089" ID="ID_324262103" MODIFIED="1382817952024" TEXT="has max length"/>
<node CREATED="1382817953819" ID="ID_445897711" MODIFIED="1382817966071" TEXT="statically allocated size"/>
</node>
<node CREATED="1382817904210" ID="ID_412010532" MODIFIED="1383424333855" TEXT="the task wait queues (actually lists in our case or priority queues)">
<icon BUILTIN="button_ok"/>
<node CREATED="1382817968272" ID="ID_1793787267" MODIFIED="1382817973530" TEXT="takes task * elements"/>
<node CREATED="1382817975338" ID="ID_1746888310" MODIFIED="1382817983107" TEXT="no need to have max length I thing"/>
<node CREATED="1382817984901" ID="ID_986914219" MODIFIED="1382817992374" TEXT="dynamically allocated, "/>
</node>
</node>
</node>
<node CREATED="1381963002236" ID="ID_1066844665" MODIFIED="1382906763270" TEXT="idle task">
<icon BUILTIN="button_ok"/>
</node>
<node CREATED="1381696351510" ID="ID_468131309" MODIFIED="1383692750441" TEXT="semaphores">
<icon BUILTIN="button_ok"/>
</node>
</node>
<node CREATED="1383511650137" HGAP="220" ID="ID_23181217" MODIFIED="1383511675466" POSITION="left" TEXT="coding conventions" VSHIFT="-95">
<node CREATED="1383511676548" ID="ID_494516949" MODIFIED="1383511682562" TEXT="local variable naming"/>
<node CREATED="1383511685507" ID="ID_1067598614" MODIFIED="1383511691579" TEXT="global variable naming"/>
<node CREATED="1383511692854" ID="ID_1703452200" MODIFIED="1383511695011" TEXT="function names"/>
</node>
<node CREATED="1383513371761" HGAP="229" ID="ID_502017828" MODIFIED="1383513392548" POSITION="left" TEXT="time to start testing" VSHIFT="-87"/>
<node CREATED="1379869115741" ID="ID_482536034" MODIFIED="1383513388468" POSITION="left" TEXT="Clean Up Task">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      On-going, periodic, recurrent
    </p>
  </body>
</html></richcontent>
<node CREATED="1379869246490" ID="ID_571819903" MODIFIED="1379869294708" TEXT="Update includes maps if needed"/>
<node CREATED="1379869139423" ID="ID_1427970198" MODIFIED="1379869154368" TEXT="Change names according to naming convention"/>
<node CREATED="1379869159044" ID="ID_410819294" MODIFIED="1379869173179" TEXT="Rearrange code to the correct modules"/>
<node CREATED="1379869176318" ID="ID_8731758" MODIFIED="1379869194877" TEXT="Variable type correction. Use kernel typedefs"/>
<node CREATED="1379869197858" ID="ID_1560770886" MODIFIED="1379869200042" TEXT="Comments"/>
<node CREATED="1379869202455" ID="ID_1674967420" MODIFIED="1379869211754" TEXT="testing of untested modules"/>
<node CREATED="1379869219879" ID="ID_275853333" MODIFIED="1379869224763" TEXT="remove unnecessary code"/>
<node CREATED="1380409653165" ID="ID_1884152085" MODIFIED="1383511668340" TEXT="Refactor-Redistribute code to files">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      for example in task.c only task management api should be defined. Everything else should be somewhere else
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node CREATED="1379869261369" ID="ID_768448576" MODIFIED="1379869264546" POSITION="right" TEXT="Documentation Steps">
<node CREATED="1379869279063" ID="ID_427638746" MODIFIED="1379869290857" TEXT="Create include map"/>
</node>
<node CREATED="1380405046607" ID="ID_1438398998" MODIFIED="1380405049060" POSITION="left" TEXT="Hooks">
<node CREATED="1380405050615" ID="ID_1235205862" MODIFIED="1380405057218" TEXT="TaskSwitchHook"/>
<node CREATED="1380405058673" ID="ID_1572616508" MODIFIED="1380405065995" TEXT="OsTickHook"/>
<node CREATED="1380405068596" ID="ID_476822085" MODIFIED="1380405085553" TEXT="klp\"/>
</node>
</node>
</map>
