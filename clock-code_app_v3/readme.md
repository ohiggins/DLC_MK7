GET
===
/status
example responses:
{ "lampOn": 0, "alarmSet": 1, "alarmHour": 22, "alarmMin": 30 }
{ "lampOn": 1, "alarmSet": 0 }

hour range:   1 - 24
minute range: 0 - 59

POST
====
/lamp
example post data:
{ "lampOn": 0 }
{ "lampOn": 1 }

/set
example post data:
{ "alarmSet": 1, "alarmHour": 3, "alarmMin": 7 }
{ "alarmSet": 0 }

MOCK SERVER
===========
https://f547a6d1-7c45-4812-b1ac-7f6f1cee1316.mock.pstmn.io

/status
returns json:
{ "lampOn": 0, "alarmSet": 1, "alarmHour": 22, "alarmMin": 30 }

/status2
returns json:: 
{ "lampOn": 1, "alarmSet": 1, "alarmHour": 3, "alarmMin": 15 }

/status3
returns json: 
{ "lampOn": 1, "alarmSet": 0 }

/lamp
returns json:
{ "success": 1 }

/set
returns json:
{ "success": 1 }