---------------------------------
--流量检定结果记录表（包括质量法和标准表法）
---------------------------------
drop table if exists "T_Flow_Verify_Record"
;
create table T_Flow_Verify_Record
(
F_ID integer not null primary key autoincrement,
F_TimeStamp timestamp not null,     --时间戳（'yyyy-MM-dd HH:mm:ss.zzz')
F_MeterNo varchar(16) not null,     --表号(14位数字: 6 + 8)
F_FlowPointIdx smallint not null,   --流量点索引，例如1,2,3,4...
F_FlowPoint float,                  --流量点数值，单位m3/h
F_MethodFlag smallint,              --方法标志：0(质量法)；1(标准表法)
F_MeterValue0 float,                --热量表初值（体积），单位L
F_MeterValue1 float,                --热量表终值（体积），单位L
F_BalWeight0 float,                 --天平初值（重量），单位kg（质量法使用）
F_BalWeight1 float,                 --天平终值（重量），单位kg（质量法使用）
F_StdMeterV0 float,                 --标准表初值（体积），单位L（标准表法使用）
F_StdMeterV1 float,                 --标准表终值（体积），单位L（标准表法使用）
F_PipeTemper float,                 --分配到每个表位的温度，单位℃
F_Density float,                    --分配到每个表位的密度，单位kg/L
F_StandValue float,                 --经过修正的标准值，单位L
F_DispError float,                  --示值误差，单位%
F_StdError float,                   --要求误差(合格标准),单位%
F_Result smallint,                  --检定结果(1：合格，0：不合格)
F_MeterPosNo smallint,              --表位号
F_Model integer,                    --表型号，外键(T_Meter_Model.F_ID)
F_Standard integer,                 --表规格(DN15/DN20/DN25)，外键(T_Meter_Standard.F_ID)
F_PickCode integer,                 --表类型(采集代码)，外键(T_Meter_PickCode.F_ID)
F_ManufactDept integer,             --制造单位，外键(T_Manufacture_Dept.F_ID)
F_VerifyDept integer,               --送检单位，外键(T_Verify_Dept.F_ID)
F_Grade smallint,                   --计量等级（1,2,3）
F_VerifyPerson integer,             --检定员，外键(T_User_Def_Tab.F_ID)
F_CheckPerson integer,              --核验员，外键(T_User_Def_Tab.F_ID)
F_DeviceInfoID integer,             --检定装置设备信息ID，外键(T_Verify_Device_Info.F_ID)
F_VerifyDate date,             	    --检定日期('2014-05-22')
F_ValidDate date,                   --检表结果有效期(生产型不需要)('2014-08-07')
F_EnvTemper float,                  --环境温度，单位℃
F_EnvHumidity float,                --环境湿度
F_AirPressure float,                --大气压力
F_CertNO varchar(16),               --检定证书编号(检表过程不写编号，留给用户以后填写)
F_FlowCoe float,                    --流量系数
F_Bak2 varchar(24),                 --备用域2
F_Bak3 varchar(24),                 --备用域3
F_Bak4 varchar(24),                 --备用域4
constraint F_Model_fk foreign key(F_Model) references T_Meter_Model(F_ID),
constraint F_Standard_fk foreign key(F_Standard) references T_Meter_Standard(F_ID),
constraint F_PickCode_fk foreign key(F_PickCode) references T_Meter_PickCode(F_ID),
constraint F_ManufactDept_fk foreign key(F_ManufactDept) references T_Manufacture_Dept(F_ID),
constraint F_VerifyDept_fk foreign key(F_VerifyDept) references T_Verify_Dept(F_ID),
constraint F_VerifyPerson_fk foreign key(F_VerifyPerson) references T_User_Def_Tab(F_ID),
constraint F_CheckPerson_fk foreign key(F_CheckPerson) references T_User_Def_Tab(F_ID),
constraint F_DeviceInfoID_fk foreign key(F_DeviceInfoID) references T_Verify_Device_Info(F_ID)
);
create unique index uk_T_Flow_Verify_Record on T_Flow_Verify_Record (F_MeterNo, F_TimeStamp, F_FlowPointIdx, F_MeterPosNo, F_DeviceInfoID);


---------------------------------
--总量检定结果记录表（包括质量法和标准表法）
---------------------------------
drop table if exists "T_Total_Verify_Record"
;
create table T_Total_Verify_Record
(
F_ID integer not null primary key autoincrement,
F_TimeStamp timestamp not null,     --时间戳（'yyyy-MM-dd HH:mm:ss.zzz')
F_MeterNo varchar(16) not null,     --表号(14位数字: 6 + 8)
F_FlowPointIdx smallint not null,   --流量点索引，例如1,2,3,4...
F_FlowPoint float,                  --流量点数值，单位m3/h
F_MethodFlag smallint,              --方法标志：0(质量法)；1(标准表法)
F_MeterValue0 float,                --热量表初值（热量），单位kwh
F_MeterValue1 float,                --热量表终值（热量），单位kwh
F_BalWeight0 float,                 --天平初值（重量），单位kg（质量法使用）
F_BalWeight1 float,                 --天平终值（重量），单位kg（质量法使用）
F_StdMeterV0 float,                 --标准表初值（体积），单位L（标准表法使用）
F_StdMeterV1 float,                 --标准表终值（体积），单位L（标准表法使用）
F_InSlotTemper float,              	--恒温槽入口温度，单位℃
F_OutSlotTemper float,              --恒温槽出口温度，单位℃
F_PipeTemper float,                 --分配到每个表位的温度，单位℃
F_Density float,                    --分配到每个表位的密度，单位kg/L
F_StandValue float,                 --经过修正的标准值，单位kwh
F_DispError float,                  --示值误差，单位%
F_StdError float,                   --要求误差(合格标准),单位%
F_Result smallint,                  --检定结果（1：合格，0：不合格）
F_MeterPosNo smallint,              --表位号
F_Model integer,                    --表型号，外键(T_Meter_Model.F_ID)
F_Standard integer,                 --表规格(DN15/DN20/DN25)，外键(T_Meter_Standard.F_ID)
F_PickCode integer,                 --表类型(采集代码)，外键(T_Meter_PickCode.F_ID)
F_ManufactDept integer,             --制造单位，外键(T_Manufacture_Dept.F_ID)
F_VerifyDept integer,               --送检单位，外键(T_Verify_Dept.F_ID)
F_Grade smallint,                   --计量等级（1,2,3）
F_VerifyPerson integer,             --检定员，外键(T_User_Def_Tab.F_ID)
F_CheckPerson integer,              --核验员，外键(T_User_Def_Tab.F_ID)
F_DeviceInfoID integer,             --检定装置设备信息ID，外键(T_Verify_Device_Info.F_ID)
F_VerifyDate date,             	    --检定日期('2014-05-22')
F_ValidDate date,                   --检表结果有效期(生产型不需要)('2014-08-07')
F_EnvTemper float,                  --环境温度，单位℃
F_EnvHumidity float,                --环境湿度
F_AirPressure float,                --大气压力
F_CertNO varchar(16),               --检定证书编号(检表过程不写编号，留给用户以后填写)
F_TotalCoe varchar(24),             --总量系数
F_Bak2 varchar(24),                 --备用域2
F_Bak3 varchar(24),                 --备用域3
F_Bak4 varchar(24),                 --备用域4
constraint F_Model_fk foreign key(F_Model) references T_Meter_Model(F_ID),
constraint F_Standard_fk foreign key(F_Standard) references T_Meter_Standard(F_ID),
constraint F_PickCode_fk foreign key(F_PickCode) references T_Meter_PickCode(F_ID),
constraint F_ManufactDept_fk foreign key(F_ManufactDept) references T_Manufacture_Dept(F_ID),
constraint F_VerifyDept_fk foreign key(F_VerifyDept) references T_Verify_Dept(F_ID),
constraint F_VerifyPerson_fk foreign key(F_VerifyPerson) references T_User_Def_Tab(F_ID),
constraint F_CheckPerson_fk foreign key(F_CheckPerson) references T_User_Def_Tab(F_ID),
constraint F_DeviceInfoID_fk foreign key(F_DeviceInfoID) references T_Verify_Device_Info(F_ID)
);
create unique index uk_T_Total_Verify_Record on T_Total_Verify_Record (F_MeterNo, F_TimeStamp, F_FlowPointIdx, F_MeterPosNo, F_DeviceInfoID);


---------------------------------
--铂电阻检定结果记录表
---------------------------------
drop table if exists "T_Platinum_Verify_Record"
;
create table T_Platinum_Verify_Record
(
	F_ID	integer not null primary key autoincrement,
	F_TimeStamp timestamp not null,     --时间戳（'yyyy-MM-dd HH:mm:ss.zzz')
	F_CompOrParam integer,              --比较法或参数法, 0-比较法, 1-参数法
	F_PlaManufactDept integer,          --被检铂电阻制造单位，外键(T_Manufacture_Dept.F_ID)
	F_Standard integer,                 --表规格(DN15/DN20/DN25)，外键(T_Meter_Standard.F_ID)
	F_Model integer,                    --表型号，外键(T_Meter_Model.F_ID)
	F_ManufactDept integer,             --热量表制造单位，外键(T_Manufacture_Dept.F_ID)
	F_VerifyDept integer,               --送检单位，外键(T_Verify_Dept.F_ID)
	F_CheckPerson integer,              --检定员，外键(T_User_Def_Tab.F_ID)
	F_VerifyPerson integer,             --核验员，外键(T_User_Def_Tab.F_ID)
	F_MinTmpDiff float,                 --最小温差, 参数
	F_TmpDiff float,                    --检测温差(比较法)/检测温度点(参数法)
	F_StdModel varchar(50),             --标准温度计型号
	F_StdInRresis float,                --标准温度计进口电阻值
	F_StdOutRresis float,               --标准温度计出口电阻值
	F_StdInTmp float,                   --标准温度计进口电温度
	F_StdOutTmp float,                  --标准温度计出口电温度
	F_PlaSerial varchar(50),            --被检铂电阻序列号
	F_PlaInRresis float,                --被检铂电阻进口电阻值
	F_PlaOutRresis float,               --被检铂电阻出口电阻值
	F_PlaInTmp float,                   --被检铂电阻进口温度值
	F_PlaOutTmp float,                  --被检铂电阻出口温度值
	F_PlaTmpDiffErr float,              --被检铂电阻进出口温差的误差	
	F_PlaParamR0 float,                 --被检铂电阻0℃电阻值
	F_PlaCoeA float,                    --被检铂电阻系数A
	F_PlaCoeB float,                    --被检铂电阻系数B
	F_PlaCoeC float,                    --被检铂电阻系数C
	F_InErr	float,                      --被检铂电阻进口误差(℃)
	F_OutErr float,                     --被检铂电阻出口误差(℃)
	F_DeltaErrLimit float,              --被检铂电阻进出口温差的误差限(%)
	F_InErrLimit float,                 --被检铂电阻进口误差限(℃)
	F_OutErrLimit float,                --被检铂电阻出口误差限(℃)
	F_verify_seq integer,               --第几次检定
	F_TmpIndex integer,                 --温差点索引(比较法)/温度点索引(参数法)
	F_MaxErrPoint float,                --最大误差点
	constraint F_Model_fk foreign key(F_Model) references T_Meter_Model(F_ID),
	constraint F_Standard_fk foreign key(F_Standard) references T_Meter_Standard(F_ID),
	constraint F_ManufactDept_fk foreign key(F_ManufactDept) references T_Manufacture_Dept(F_ID),
	constraint F_VerifyDept_fk foreign key(F_VerifyDept) references T_Verify_Dept(F_ID),
	constraint F_VerifyPerson_fk foreign key(F_VerifyPerson) references T_User_Def_Tab(F_ID),
	constraint F_CheckPerson_fk foreign key(F_CheckPerson) references T_User_Def_Tab(F_ID)
);


---------------------------------
--计算器检定结果记录表
---------------------------------
drop table if exists "T_Calc_Verify_Record"
;
create table T_Calc_Verify_Record
(
F_ID integer not null primary key autoincrement,
F_TimeStamp timestamp not null,     --时间戳（'yyyy-MM-dd HH:mm:ss.zzz')
F_MeterNo varchar(16) not null,     --表号(14位数字: 6 + 8)
F_DeltaTIdx smallint not null,      --温差点索引，例如1,2,3,4...
F_Standard integer,                 --表规格(DN15/DN20/DN25)，外键(T_Meter_Standard.F_ID)
F_Model integer,                    --表型号，外键(T_Meter_Model.F_ID)
F_Grade smallint,                   --计量等级（1,2,3）
F_ManufactDept integer,             --制造单位，外键(T_Manufacture_Dept.F_ID)
F_VerifyDept integer,               --送检单位，外键(T_Verify_Dept.F_ID)
F_VerifyPerson integer,             --检定员，外键(T_User_Def_Tab.F_ID)
F_MaxT float,                       --最高温度(℃)
F_MinT float,                       --最小温度(℃)
F_MaxDeltaT float,                  --最大温差(K)
F_MinDeltaT float,                  --最小温差(K)
F_Algorithm integer,                --算法(0:焓差法； 1:K系数法)
F_InstallPos integer,               --安装位置(0:进口；1:出口)
F_EnergyUnit integer,               --热量单位(0:MJ; 1:kWh)
F_StdTempIn float,                  --入口温度-标准温度计(℃)
F_StdTempOut float,                 --出口温度-标准温度计(℃)
F_StdResistIn float,                --入口电阻-标准温度计(Ω)
F_StdResistOut float,               --出口电阻-标准温度计(Ω)
F_RecomVolume float,                --建议体积(L)
F_AnalogVolume float,               --模拟体积(L)
F_Kcoe float,                       --K系数
F_StdEnergy float,                  --理论值(热量，kwh)
F_MeterE0 float,                    --热量表初值(热量)，单位kWh
F_MeterE1 float,                    --热量表终值(热量)，单位kWh
F_DispError float,                  --示值误差，单位%
F_StdError float,                   --要求误差(合格标准),单位%
F_Result smallint,                  --检定结果(1：合格，0：不合格)
F_Bak2 varchar(24),                 --备用域2
F_Bak3 varchar(24),                 --备用域3
F_Bak4 varchar(24),                 --备用域4
constraint F_Model_fk foreign key(F_Model) references T_Meter_Model(F_ID),
constraint F_Standard_fk foreign key(F_Standard) references T_Meter_Standard(F_ID),
constraint F_ManufactDept_fk foreign key(F_ManufactDept) references T_Manufacture_Dept(F_ID),
constraint F_VerifyDept_fk foreign key(F_VerifyDept) references T_Verify_Dept(F_ID),
constraint F_VerifyPerson_fk foreign key(F_VerifyPerson) references T_User_Def_Tab(F_ID)
);
create unique index uk_T_Calc_Verify_Record on T_Calc_Verify_Record (F_MeterNo, F_TimeStamp, F_DeltaTIdx);


---------------------------------
--计算器和铂电阻组合检定结果记录表
---------------------------------
drop table if exists "T_Combined_Verify_Record"
;
create table T_Combined_Verify_Record
(
F_ID integer not null primary key autoincrement,
F_TimeStamp timestamp not null,     --时间戳（'yyyy-MM-dd HH:mm:ss.zzz')
F_MeterNo varchar(16) not null,     --表号(14位数字: 6 + 8)
F_Standard integer,                 --表规格(DN15/DN20/DN25)，外键(T_Meter_Standard.F_ID)
F_Model integer,                    --表型号，外键(T_Meter_Model.F_ID)
F_PickCode integer,                 --采集代码, 外键(T_Meter_PickCode)
F_Grade smallint,                   --计量等级（1,2,3）
F_ManufactDept integer,             --制造单位，外键(T_Manufacture_Dept.F_ID)
F_VerifyDept integer,               --送检单位，外键(T_Verify_Dept.F_ID)
F_VerifyPerson integer,             --检定员，外键(T_User_Def_Tab.F_ID)
--F_CheckPerson  integer,             --核检员，外键(T_User_Def_Tab.F_ID)
F_DeltaTemp float,                  --温差(K)
F_VerifyVolume float,               --检定量(L)
F_DeltaTempMin float,               --最小温差(K)
F_InstallPos integer,               --安装位置(0:进口；1:出口)
F_HeatUnit integer,                 --热量单位(1:kWh; 0:MJ)
F_StdTempIn float,                  --入口温度-标准温度计(℃)
F_StdTempOut float,                 --出口温度-标准温度计(℃)
F_StdResistIn float,                --入口电阻-标准温度计(Ω)
F_StdResistOut float,               --出口电阻-标准温度计(Ω)
F_Kcoe float,                       --K系数
F_StdValue float,                   --理论值(热量，kwh)
F_MeterV0 float,                    --热量表初值(体积)，单位L
F_MeterV1 float,                    --热量表终值(体积)，单位L
F_MeterE0 float,                    --热量表初值(热量)，单位kWh
F_MeterE1 float,                    --热量表终值(热量)，单位kWh
F_DispError float,                  --示值误差，单位%
F_StdError float,                   --要求误差(合格标准),单位%
F_Result smallint,                  --检定结果(1：合格，0：不合格)
F_Bak2 varchar(24),                 --备用域2
F_Bak3 varchar(24),                 --备用域3
F_Bak4 varchar(24),                 --备用域4
constraint F_Model_fk foreign key(F_Model) references T_Meter_Model(F_ID),
constraint F_Standard_fk foreign key(F_Standard) references T_Meter_Standard(F_ID),
constraint F_ManufactDept_fk foreign key(F_ManufactDept) references T_Manufacture_Dept(F_ID),
constraint F_VerifyDept_fk foreign key(F_VerifyDept) references T_Verify_Dept(F_ID),
constraint F_VerifyPerson_fk foreign key(F_VerifyPerson) references T_User_Def_Tab(F_ID),
constraint F_PickCode_fk foreign key(F_PickCode) references T_Meter_PickCode(F_ID)
);
create unique index uk_T_Combined_Verify_Record on T_Combined_Verify_Record (F_MeterNo, F_TimeStamp);

---------------------------------
--表规格
---------------------------------
drop table if exists "T_Meter_Standard"
;
create table T_Meter_Standard
(
F_ID integer not null primary key,  --规格ID
F_Name varchar(24),                 --规格代码
F_Meter_Quantity smallint           -- 被检表的数量(2014.07.31 修改By Song baoshan)
);
insert into T_Meter_Standard(F_ID, F_Name, F_Meter_Quantity) values(0, 'DN15', 12);
insert into T_Meter_Standard(F_ID, F_Name, F_Meter_Quantity) values(1, 'DN20', 12);
insert into T_Meter_Standard(F_ID, F_Name, F_Meter_Quantity) values(2, 'DN25', 10);
insert into T_Meter_Standard(F_ID, F_Name, F_Meter_Quantity) values(3, 'DN32', 6);
insert into T_Meter_Standard(F_ID, F_Name, F_Meter_Quantity) values(4, 'DN40', 4);
insert into T_Meter_Standard(F_ID, F_Name, F_Meter_Quantity) values(5, 'DN50', 4);

---------------------------------
--热表各规格的默认参数表
---------------------------------
drop table if exists "T_Meter_Default_Params"
;
create table T_Meter_Default_Params
(
	F_ID integer not null primary key,      -- 参数id
	F_StandardID integer,                   -- 外键,表规格id(T_Meter_Standard.F_ID)
	F_NormalFlow float,                     -- 常用流量点
	F_UpperFlow float,                      -- 上限流量点		
	F_VerifyFlow float,                     -- 检定流量点
	F_FlowQuantity float,                   -- 检定量
	F_PumpFrequencey float,                 -- 水泵频率
	F_Valve_i smallint,                     -- 阀门编号	
	F_Seq_i smallint,                       -- 检测序列号
	F_Opening smallint,                     -- 调节阀开度
  constraint F_StandardID_fk foreign key(F_StandardID) references T_Meter_Standard(F_ID)
);
insert into T_Meter_Default_Params values(0,  0, 1.5, 30,   1.5,  40, 34, 0, 1, 50);
insert into T_Meter_Default_Params values(1,  0, 1.5, 7,    0.45, 20, 33, 1, 2, 50);
insert into T_Meter_Default_Params values(2,  0, 1.5, 1.05, 0.15, 10, 33, 2, 3, 50);
insert into T_Meter_Default_Params values(3,  0, 1.5, 0.15, 0.03, 5,  33, 3, 4, 50);
insert into T_Meter_Default_Params values(4,  1, 2.5, 30,   2.5,  50, 34, 0, 1, 50);
insert into T_Meter_Default_Params values(5,  1, 2.5, 7,    0.75, 20, 33, 1, 2, 50);
insert into T_Meter_Default_Params values(6,  1, 2.5, 1.05, 0.25, 10, 33, 2, 3, 50);
insert into T_Meter_Default_Params values(7,  1, 2.5, 0.15, 0.05, 5,  33, 3, 4, 50);
insert into T_Meter_Default_Params values(8,  2, 3.5, 30,   3.5,  50, 34, 0, 1, 50);
insert into T_Meter_Default_Params values(9,  2, 3.5, 7,    1.05, 20, 33, 1, 2, 50);
insert into T_Meter_Default_Params values(10, 2, 3.5, 1.05, 0.35, 10, 33, 2, 3, 50);
insert into T_Meter_Default_Params values(11, 2, 3.5, 0.15, 0.07, 5,  33, 3, 4, 50);

insert into T_Meter_Default_Params values(12, 3, 6.0, 30,    6.0,  150, 34, 0, 1, 50);
insert into T_Meter_Default_Params values(13, 3, 6.0, 7,     1.8,  30,  33, 1, 2, 50);
insert into T_Meter_Default_Params values(14, 3, 6.0, 1.05,  0.6,  20,  33, 2, 3, 50);
insert into T_Meter_Default_Params values(15, 3, 6.0, 0.15,  0.12, 10,  33, 3, 4, 50);
insert into T_Meter_Default_Params values(16, 4, 10,  30,    10,   350, 34, 0, 1, 50);
insert into T_Meter_Default_Params values(17, 4, 10,  7,     3,    35,  33, 1, 2, 50);
insert into T_Meter_Default_Params values(18, 4, 10,  1.05,  1,    25,  33, 2, 3, 50);
insert into T_Meter_Default_Params values(19, 4, 10,  0.15,  0.2,  15,  33, 3, 4, 50);
insert into T_Meter_Default_Params values(20, 5, 15,  30,    15,   450, 34, 0, 1, 50);
insert into T_Meter_Default_Params values(21, 5, 15,  7,     4.5,  40,  33, 1, 2, 50);
insert into T_Meter_Default_Params values(22, 5, 15,  1.05,  1.5,  30,  33, 2, 3, 50);
insert into T_Meter_Default_Params values(23, 5, 15,  0.15,  0.3,  20,  33, 3, 4, 50);

---------------------------------
--表型号
---------------------------------
drop table if exists "T_Meter_Model"
;
create table T_Meter_Model
(
F_ID integer not null primary key autoincrement,  --型号ID
F_Name varchar(24),
F_Desc varchar(60)
);
insert into T_Meter_Model(F_ID, F_Name, F_Desc) values(0, 'E-UWZ1', '型号1');
insert into T_Meter_Model(F_ID, F_Name, F_Desc) values(1, 'E-UWZ2', '型号2');
insert into T_Meter_Model(F_ID, F_Name, F_Desc) values(2, 'E-UWZ3', '型号3');


---------------------------------
--表类型/采集代码
---------------------------------
drop table if exists "T_Meter_PickCode"
;
create table T_Meter_PickCode
(
F_ID integer not null primary key,
F_Desc varchar(60),
F_NumPrefix varchar(6)
);
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(0, '通用', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(1, '德鲁超声波表', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(2, '天罡超声波表', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(3, '力创超声波表', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(4, '力创机械表', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(5, '汇中超声波表', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(6, '迈拓超声波表', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(7, '瑞纳超声波表', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(8, '光大', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(9, '荷德鲁美特', '111100');
insert into T_Meter_PickCode(F_ID, F_Desc, F_NumPrefix) values(10, '航天德鲁热量表', '111100');


---------------------------------
--制造单位
---------------------------------
drop table if exists "T_Manufacture_Dept"
;
create table T_Manufacture_Dept
(
F_ID integer not null primary key,
F_Name varchar(24),
F_Desc varchar(60)
);
insert into T_Manufacture_Dept(F_ID, F_Name, F_Desc) values(0, 'SDM', '山东德鲁计量');
insert into T_Manufacture_Dept(F_ID, F_Name, F_Desc) values(1, 'ADE', '烟台航天德鲁');
insert into T_Manufacture_Dept(F_ID, F_Name, F_Desc) values(2, 'SDJG', '烟台晶格');
insert into T_Manufacture_Dept(F_ID, F_Name, F_Desc) values(3, 'HYLY', '华仪乐业');


---------------------------------
--送检单位
---------------------------------
drop table if exists "T_Verify_Dept"
;
create table T_Verify_Dept
(
F_ID integer not null primary key,
F_Name varchar(24),
F_Desc varchar(60)
);
insert into T_Verify_Dept(F_ID, F_Name, F_Desc) values(0, 'SDJL', '山东计量院');
insert into T_Verify_Dept(F_ID, F_Name, F_Desc) values(1, 'QDJL', '青岛计量所');
insert into T_Verify_Dept(F_ID, F_Name, F_Desc) values(2, 'YTJL', '烟台计量所');
insert into T_Verify_Dept(F_ID, F_Name, F_Desc) values(3, 'SDM', '山东德鲁计量');
insert into T_Verify_Dept(F_ID, F_Name, F_Desc) values(4, 'ADE', '烟台航天德鲁');


---------------------------------
--角色定义表
---------------------------------
drop table if exists "T_Role_Def_Tab"
;
create table T_Role_Def_Tab
(
F_ID integer not null primary key,
F_Name varchar(24),
F_Desc varchar(60),
F_Func1 smallint,
F_Func2 smallint,
F_Func3 smallint
);
insert into T_Role_Def_Tab(F_ID, F_Name, F_Desc) values(0, 'root', '超级用户');
insert into T_Role_Def_Tab(F_ID, F_Name, F_Desc) values(1, 'common', '普通用户');
insert into T_Role_Def_Tab(F_ID, F_Name, F_Desc) values(2, 'repair', '维修人员');


---------------------------------
--用户定义表
---------------------------------
drop table if exists "T_User_Def_Tab"
;
create table T_User_Def_Tab
(
F_ID integer not null primary key,
F_Name varchar(24),
F_Desc varchar(60) not null,
F_Password varchar(24),
F_RoleID integer not null,       --角色ID，外键(T_Role_Def_Tab)
constraint F_RoleID_fk foreign key(F_RoleID) references T_Role_Def_Tab(F_ID)
);
insert into T_User_Def_Tab(F_ID, F_Name, F_Desc, F_Password, F_RoleID) values(0, 'admin', '管理员', 'admin', 0);
insert into T_User_Def_Tab(F_ID, F_Name, F_Desc, F_Password, F_RoleID) values(1, 'verifier', '检定员', '1234', 1);
insert into T_User_Def_Tab(F_ID, F_Name, F_Desc, F_Password, F_RoleID) values(2, 'checker', '核验员', '1234', 1);


---------------------------------
--是否表
---------------------------------
drop table if exists "T_Yes_No_Tab"
;
create table T_Yes_No_Tab
(
F_ID integer not null primary key,
F_Name varchar(8),
F_Desc varchar(12)
);
insert into T_Yes_No_Tab(F_ID, F_Name, F_Desc) values(0, 'No', '不合格');
insert into T_Yes_No_Tab(F_ID, F_Name, F_Desc) values(1, 'Yes', '合格');



---------------------------------
--检定装置信息表
---------------------------------
drop table if exists "T_Verify_Device_Info"
;
create table T_Verify_Device_Info
(
F_ID integer not null primary key,
F_DeviceName varchar(24) not null,     --标准装置名称（热量表检定装置）
F_DeviceNo varchar(24) not null,       --标准装置编号（SDM201280037)
F_DeviceModel varchar(20),             --装置型号("RJZ32/80Z/B")
F_Manufact varchar(24),                --制造厂商("德鲁计量")
F_DeviceGrade varchar(24),             --装置的准确度等级("0.1% k=2")
F_MeasureRange varchar(24),            --装置的测量范围("0.12-40.0(m3/h)")
F_CertNo integer not null,             --标准装置证书编号
F_VerifyRule varchar(20),              --检定规程("JJG225-2001")
F_DeviceValidDate date,                --标准装置有效期('2014-08-07')
F_CertValidDate date,                  --证书有效期('2014-08-07')
F_RuleValidDate date,                  --计量标准考核证书有效期('2014-08-07')
F_Bak1 varchar(24),                    --备用域1
F_Bak2 varchar(24),                    --备用域2
F_Bak3 varchar(24),                    --备用域3
F_Bak4 varchar(24)                     --备用域4
);
create unique index uk_T_Verify_Device_Info on T_Verify_Device_Info (F_DeviceNo);
insert into T_Verify_Device_Info values(1, '航天德鲁检定装置', 'ADE201580037', 'RJZ15-25Z/B', '航天德鲁', '0.1% k=2', '0.12-40.0(m3/h)', 1234567890, 'JJG225-2001', '2020-7-1', '2020-7-1', '2020-7-1', '', '', '', '');
insert into T_Verify_Device_Info values(2, '航天德鲁检定装置', 'ADE201580038', 'RJZ15-25Z/B', '航天德鲁', '0.1% k=2', '0.12-40.0(m3/h)', 1234567891, 'JJG225-2001', '2020-7-1', '2020-7-1', '2020-7-1', '', '', '', '');
insert into T_Verify_Device_Info values(3, '航天德鲁检定装置', 'ADE201580039', 'RJZ15-25Z/B', '航天德鲁', '0.1% k=2', '0.12-40.0(m3/h)', 1234567892, 'JJG225-2001', '2020-7-1', '2020-7-1', '2020-7-1', '', '', '', '');
insert into T_Verify_Device_Info values(4, '航天德鲁检定装置', 'ADE201580040', 'RJZ15-25Z/B', '航天德鲁', '0.1% k=2', '0.12-40.0(m3/h)', 1234567893, 'JJG225-2001', '2020-7-1', '2020-7-1', '2020-7-1', '', '', '', '');
insert into T_Verify_Device_Info values(5, '航天德鲁检定装置', 'ADE201580041', 'RJZ15-25Z/B', '航天德鲁', '0.1% k=2', '0.12-40.0(m3/h)', 1234567894, 'JJG225-2001', '2020-7-1', '2020-7-1', '2020-7-1', '', '', '', '');
insert into T_Verify_Device_Info values(6, '航天德鲁检定装置', 'ADE201580042', 'RJZ15-25Z/B', '航天德鲁', '0.1% k=2', '0.12-40.0(m3/h)', 1234567895, 'JJG225-2001', '2020-7-1', '2020-7-1', '2020-7-1', '', '', '', '');

-----------------------------------------------------------------
--                    临时视图创建语句表                     ----
-----------------------------------------------------------------
drop table if exists T_Create_Query_View_Stmt;
create table T_Create_Query_View_Stmt
(
  F_ID integer not null primary key autoincrement,
  F_DESC varchar(50),  
  F_STMT varchar(5000)
);

INSERT INTO T_Create_Query_View_Stmt
(F_ID, F_DESC, F_STMT) values
(
1,
 'temp flow_verify query result view', 
 'CREATE view V_Temp_Flow_Query_Result as
select
  rec.F_ID F_RowId,
  rec.F_TimeStamp,
  substr(rec.F_TimeStamp,1,16) F_TimeStamp_short,
  substr(rec.F_MeterNo,7,8) F_MeterNo,
  rec.F_FlowPointIdx,
  round(rec.F_FlowPoint,3) F_FlowPoint,
  rec.F_MethodFlag,
  rec.F_MeterValue0,
  rec.F_MeterValue1,
  round((rec.F_MeterValue1 - rec.F_MeterValue0), 2) F_MeterDispValue,
  rec.F_BalWeight0,
  rec.F_BalWeight1,
  round((rec.F_BalWeight1 - rec.F_BalWeight0), 2) F_BalDisp,
  rec.F_StdMeterV0,
  rec.F_StdMeterV1,
  round((rec.F_StdMeterV1 - rec.F_StdMeterV0), 2) F_StdDisp,
  rec.F_PipeTemper,
  rec.F_Density,
  rec.F_StandValue,
  rec.F_DispError,
  rec.F_StdError,
  rec.F_Grade,
  (select F_Desc from T_User_Def_Tab u where u.F_id = rec.F_VerifyPerson)  F_VerifyPerson,
  (select F_Desc from T_User_Def_Tab u where u.F_id = rec.F_CheckPerson)  F_CheckPerson,
  rec.F_DeviceInfoID,
  rec.F_VerifyDate,
  rec.F_ValidDate,
  rec.F_EnvTemper,
  rec.F_EnvHumidity,
  rec.F_AirPressure,
  rec.F_FlowCoe,
  rec.F_Result,
  rec.F_MeterPosNo,
  rec.F_Model,
  mo.F_Name F_Model_en,
  mo.F_Desc F_Model_zh,
  st.f_name F_Standard,
  tp.F_Desc F_PickCode_zh,
  manu.F_Name F_ManufactDept_en,
  manu.F_Desc F_ManufactDept_zh,
  vdpt.F_Name F_VerifyDept_en,
  vdpt.F_Desc F_VerifyDept_zh,
  yesno.F_Name valid_en, 
  yesno.F_Desc valid,
  d.F_CertNO,
  d.F_DeviceName,
  d.F_DeviceNo,
  d.F_DeviceModel,
  d.F_Manufact,
  d.F_DeviceGrade,
  d.F_MeasureRange,
  d.F_VerifyRule,
  d.F_DeviceValidDate,
  d.F_CertValidDate,
  d.F_RuleValidDate
	from
	T_Temp_Query_Result rec left join
	T_Meter_Model mo on rec.F_Model=mo.F_ID left join
    T_Meter_Standard st on rec.F_Standard=st.F_ID left join
	T_Meter_PickCode tp on rec.F_PickCode=tp.F_ID left join
	T_Manufacture_Dept manu on rec.F_ManufactDept=manu.F_ID left join
	T_Verify_Dept vdpt on rec.F_VerifyDept=vdpt.F_ID left join
	T_Yes_No_Tab  yesno on rec.F_Result=yesno.F_ID left join
    T_User_Def_Tab usert on rec.F_VerifyPerson=usert.F_ID left join
	T_Verify_Device_Info d on rec.F_DeviceInfoID=d.F_ID
	order by rec.F_MeterPosNo, rec.f_meterno, rec.f_timestamp
;'
);

INSERT INTO T_Create_Query_View_Stmt
(F_ID, F_DESC, F_STMT) values
(
2,
 'temp combined_verify query result view', 
 'CREATE view V_Temp_Cmb_Query_Result as
select
	rec.F_ID,
    rec.F_TimeStamp,
    substr(rec.F_TimeStamp,1,16) F_TimeStamp_short,
	substr(rec.F_MeterNo,7,8) F_MeterNo,
	rec.F_Grade,
	rec.F_DeltaTemp,
	rec.F_VerifyVolume,
	rec.F_DeltaTempMin,
	rec.F_InstallPos,
	rec.F_HeatUnit,
	rec.F_StdTempIn,
	rec.F_StdTempOut,
	rec.F_StdResistIn,
	rec.F_StdResistOut,
	rec.F_Kcoe,
	rec.F_StdValue,
	rec.F_MeterV0,
	rec.F_MeterV1,
	rec.F_MeterE0,
	rec.F_MeterE1,
	round((rec.F_MeterE1-rec.F_MeterE0), 2) F_MeterDispV,
	rec.F_DispError,
	rec.F_StdError,
	date(rec.F_TimeStamp) F_VerifyDate,
	yesno.F_Desc F_Result,  
	yesno.F_Name F_result_en,
	usert.F_Desc F_VerifyPerson,
	yesno.F_Name valid_en, 
	yesno.F_Desc valid,
	mo.F_Name F_Model,
	mo.F_Desc F_Model_zh,
	st.f_name F_Standard,
	tp.F_Desc F_PickCode,
	manu.F_Name F_ManufactDept_en,
	manu.F_Desc F_ManufactDept,
	vdpt.F_Name F_VerifyDept_en,
	vdpt.F_Desc F_VerifyDept
from
	T_Temp_Query_Result rec left join
	T_Meter_Model mo on rec.F_Model=mo.F_ID left join
	T_Meter_Standard st on rec.F_Standard=st.F_ID left join
	T_Meter_PickCode tp on rec.F_PickCode=tp.F_ID left join
	T_Manufacture_Dept manu on rec.F_ManufactDept=manu.F_ID left join
	T_Verify_Dept vdpt on rec.F_VerifyDept=vdpt.F_ID left join
	T_Yes_No_Tab  yesno on rec.F_Result=yesno.F_ID left join
    T_User_Def_Tab usert on rec.F_VerifyPerson=usert.F_ID
	order by rec.F_Meterno, rec.F_TimesTamp
;'
);

INSERT INTO T_Create_Query_View_Stmt
(F_ID, F_DESC, F_STMT) values
(
3,
 'temp total_verify query result view', 
'CREATE view V_Temp_Total_Query_Result as
select
	rec.F_ID,
    rec.F_TimeStamp,
    substr(rec.F_TimeStamp,1,16) F_TimeStamp_short,
	substr(rec.F_MeterNo,7,8) F_MeterNo,
	rec.F_FlowPointIdx,
	round(rec.F_FlowPoint,3) F_FlowPoint,
	rec.F_MethodFlag,
	rec.F_MeterValue0,
	rec.F_MeterValue1,
	rec.F_BalWeight0,
    rec.F_BalWeight1,
    round((rec.F_BalWeight1 - rec.F_BalWeight0), 2) F_BalDisp,
    rec.F_StdMeterV0,
    rec.F_StdMeterV1,
    round((rec.F_StdMeterV1 - rec.F_StdMeterV0), 2) F_StdDisp,
	rec.F_InSlotTemper,
	rec.F_OutSlotTemper,
	rec.F_PipeTemper,
	rec.F_Density,
	rec.F_StandValue,
	rec.F_DispError,
	rec.F_StdError,
	rec.F_Result,
	rec.F_MeterPosNo,
	rec.F_Model,
	rec.F_Standard,
	rec.F_PickCode,
	rec.F_ManufactDept,
	rec.F_VerifyDept,
	rec.F_Grade,
	rec.F_DeviceInfoID,
	rec.F_VerifyDate,
	rec.F_ValidDate,
	rec.F_EnvTemper,
	rec.F_EnvHumidity,
	rec.F_AirPressure,
	rec.F_CertNO,
	rec.F_TotalCoe,
	rec.F_InSlotTemper,
	rec.F_OutSlotTemper,
	(rec.F_InSlotTemper||''/''||rec.F_OutSlotTemper) F_SlotInOutTemp,
	(rec.F_InSlotTemper-rec.F_OutSlotTemper) F_SlotTempDiff,
	(rec.F_MeterValue1-rec.F_MeterValue0) F_MeterDispValue,
	d.F_CertNO,
	d.F_DeviceName,
	d.F_DeviceNo,
	d.F_DeviceModel,
	d.F_Manufact,
	d.F_DeviceGrade,
	d.F_MeasureRange,
	d.F_VerifyRule,
	d.F_DeviceValidDate,
	d.F_CertValidDate,
	d.F_RuleValidDate,  
	(select F_Desc from T_User_Def_Tab u where u.F_id = rec.F_VerifyPerson)  F_VerifyPerson,
	(select F_Desc from T_User_Def_Tab u where u.F_id = rec.F_CheckPerson)  F_CheckPerson,  
	mo.F_Name F_Model_en,
	mo.F_Desc F_Model_zh,
	st.f_name F_Standard,
	tp.F_Desc F_PickCode_zh,
	manu.F_Name F_ManufactDept_en,
	manu.F_Desc F_ManufactDept_zh,
	vdpt.F_Name F_VerifyDept_en,
	vdpt.F_Desc F_VerifyDept_zh,  
	yesno.F_Name valid_en, 
	yesno.F_Desc valid
from
	T_Temp_Query_Result rec left join
	T_Verify_Device_Info d on rec.F_DeviceInfoID=d.F_ID left join
	T_Meter_Model mo on rec.F_Model=mo.F_ID left join
	T_meter_standard st on rec.F_Standard=st.F_ID left join
	T_Meter_PickCode tp on rec.F_PickCode=tp.F_ID left join
	T_manufacture_dept manu on rec.F_ManufactDept=manu.F_ID left join
	T_verify_dept vdpt on rec.F_VerifyDept=vdpt.F_ID left join
	T_Yes_No_Tab  yesno on rec.F_Result=yesno.F_ID
	order by rec.F_MeterPosNo, rec.f_meterno, rec.f_timestamp
;'
);

INSERT INTO T_Create_Query_View_Stmt
(F_ID, F_DESC, F_STMT) values
(
4,
 'temp water_verify query result view', 
 'CREATE view V_Temp_Water_Query_Result as
select
  rec.F_ID F_RowId,
  rec.F_TimeStamp,
  substr(rec.F_TimeStamp,1,16) F_TimeStamp_short,
  substr(rec.F_MeterNo,7,8) F_MeterNo,
  rec.F_FlowPointIdx,
  round(rec.F_FlowPoint,3) F_FlowPoint,
  rec.F_MethodFlag,
  rec.F_MeterValue0,
  rec.F_MeterValue1,
  round((rec.F_MeterValue1 - rec.F_MeterValue0), 2) F_MeterDispValue,
  rec.F_BalWeight0,
  rec.F_BalWeight1,
  round((rec.F_BalWeight1 - rec.F_BalWeight0), 2) F_BalDisp,
  rec.F_StdMeterV0,
  rec.F_StdMeterV1,
  round((rec.F_StdMeterV1 - rec.F_StdMeterV0), 2) F_StdDisp,
  rec.F_PipeTemper,
  rec.F_Density,
  rec.F_StandValue,
  rec.F_DispError,
  rec.F_StdError,
  rec.F_Grade,
  (select F_Desc from T_User_Def_Tab u where u.F_id = rec.F_VerifyPerson)  F_VerifyPerson,
  (select F_Desc from T_User_Def_Tab u where u.F_id = rec.F_CheckPerson)  F_CheckPerson,
  rec.F_DeviceInfoID,
  rec.F_VerifyDate,
  rec.F_ValidDate,
  rec.F_EnvTemper,
  rec.F_EnvHumidity,
  rec.F_AirPressure,
  rec.F_FlowCoe,
  rec.F_Result,
  rec.F_MeterPosNo,
  rec.F_Model,
  rec.F_WaterPress,
  rec.F_Q3,
  rec.F_Q3BiQ1,
  rec.F_Q2BiQ1,
  mo.F_Name F_Model_en,
  mo.F_Desc F_Model_zh,
  st.f_name F_Standard,
  tp.F_Desc F_PickCode_zh,
  manu.F_Name F_ManufactDept_en,
  manu.F_Desc F_ManufactDept_zh,
  vdpt.F_Name F_VerifyDept_en,
  vdpt.F_Desc F_VerifyDept_zh,
  yesno.F_Name valid_en, 
  yesno.F_Desc valid,
  d.F_CertNO,
  d.F_DeviceName,
  d.F_DeviceNo,
  d.F_DeviceModel,
  d.F_Manufact,
  d.F_DeviceGrade,
  d.F_MeasureRange,
  d.F_VerifyRule,
  d.F_DeviceValidDate,
  d.F_CertValidDate,
  d.F_RuleValidDate
	from
	T_Temp_Query_Result rec left join
	T_Meter_Model mo on rec.F_Model=mo.F_ID left join
    T_Meter_Standard st on rec.F_Standard=st.F_ID left join
	T_Meter_PickCode tp on rec.F_PickCode=tp.F_ID left join
	T_Manufacture_Dept manu on rec.F_ManufactDept=manu.F_ID left join
	T_Verify_Dept vdpt on rec.F_VerifyDept=vdpt.F_ID left join
	T_Yes_No_Tab  yesno on rec.F_Result=yesno.F_ID left join
    T_User_Def_Tab usert on rec.F_VerifyPerson=usert.F_ID left join
	T_Verify_Device_Info d on rec.F_DeviceInfoID=d.F_ID
	order by rec.F_MeterPosNo, rec.f_meterno, rec.f_timestamp
;'
);
-----------------------------------------------------------------
--                    PID创建语句表                     ----
-----------------------------------------------------------------
drop table if exists T_PID_Setting_Record;
CREATE TABLE T_PID_Setting_Record (
	F_ID integer PRIMARY KEY AUTOINCREMENT,
	pid_timestamp timestamp,
	pid_maxRate float,
	pid_targetRate float,
	pid_currentRate float,
	pid_currentError float,
	pid_regularNo integer,
	
	pid_currentDegree float,
	pid_gainTargetRate integer,
	pid_nowErrorPercent float,
	pid_waitTime integer,
	pid_Kp float,
	pid_Ki float,
	pid_Kd float,
	pid_P float,
	pid_I float,
	pid_D float,
	
	pid_adjust_valve integer, --当前正在调节电动阀门(0, 没有; 1, 有)
	pid_adjust_pump  integer, --当前正在调节水泵频率(0, 没有; 1, 有)
	pid_pump_Kp float,
	pid_pump_Ki float,
	pid_pump_Kd float,
	pid_pump_waitTime integer,
	pid_pump_P float,
	pid_pump_I float,
	pid_pump_D float,
	
	pid_pumpFreq int
);