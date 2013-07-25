MQXPerformance
==============

Sample code to test the performance of the MQX while running on a K60

Steps to recreate with Code Warrior (CW)
1. Select new Workspace, eg ...\performanceTest
2. drag and drop ${MQX_ROOT_LOC}config/twrk60n512/cw10gcc/twrk60n512.wsd into new workspace
3. edit bsp user_config.h macro #define RTCSCFG_ENABLE_SNMP      1
4. edit psp small_ram_config.h macro #define MQX_HAS_TIME_SLICE 1
5. build MQX libraries
6. create new MQX 4.0 project: eg TestK60 (and configure using your board package, toolchain, debuger etc)
7. open xterm window and cd into project source directory (eg. .../performanceTest/TestK60 )
8. create git repository: git init
9. remove the two main files created by Code Warrior
10.add remote origin to this repository: git remote add origin https://github.com/tsbiberdorf/MQXPerformance.git
11.fetch all code: get origin fetch
12.git checkout master
  *** NOTE *** now here the trick to get "git" and "CW" to play with each other
  a. delete all files in Source
  b. create a dummy directory: toRemove
  c. cd into toRemove
  d. initalize another git repository: git init
  e. remote to github: git remote add origin https://github.com/tsbiberdorf/MQXPerformance.git
  f. git fetch origin
  g. git checkout master
  h. in CW project TestK60 select Sources folder, right mouse->import -> General->File System -> Browse to ToRemove/Sources directory
  i. select all files
  j. delete the "toRemove" directory
  
13.re-sync the git repository: git reset --hard
14.build project
15.load and run

