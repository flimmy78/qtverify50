---------------------------------
--ˮ��춨�����¼�������������ͱ�׼����
---------------------------------
drop table if exists "T_Water_Verify_Record"
;
create table T_Water_Verify_Record
(
F_ID integer not null primary key autoincrement,
F_TimeStamp timestamp not null,     --ʱ�����'yyyy-MM-dd HH:mm:ss.zzz')
F_MeterNo varchar(16) not null,     --���(14λ����: 6 + 8)
F_FlowPointIdx smallint not null,   --����������������1,2,3,4...
F_FlowPoint float,                  --��������ֵ����λm3/h
F_MethodFlag smallint,              --������־��0(������)��1(��׼��)
F_MeterValue0 float,                --�������ֵ�����������λL
F_MeterValue1 float,                --��������ֵ�����������λL
F_BalWeight0 float,                 --��ƽ��ֵ������������λkg��������ʹ�ã�
F_BalWeight1 float,                 --��ƽ��ֵ������������λkg��������ʹ�ã�
F_StdMeterV0 float,                 --��׼���ֵ�����������λL����׼��ʹ�ã�
F_StdMeterV1 float,                 --��׼����ֵ�����������λL����׼��ʹ�ã�
F_PipeTemper float,                 --���䵽ÿ����λ���¶ȣ���λ��
F_Density float,                    --���䵽ÿ����λ���ܶȣ���λkg/L
F_StandValue float,                 --���������ı�׼ֵ����λL
F_DispError float,                  --ʾֵ����λ%
F_StdError float,                   --Ҫ�����(�ϸ��׼),��λ%
F_Result smallint,                  --�춨���(1���ϸ�0�����ϸ�)
F_MeterPosNo smallint,              --��λ��
F_Model integer,                    --���ͺţ����(T_Meter_Model.F_ID)
F_Standard integer,                 --����(DN15/DN20/DN25)�����(T_Meter_Standard.F_ID)
F_PickCode integer,                 --������(�ɼ�����)�����(T_Meter_PickCode.F_ID)
F_ManufactDept integer,             --���쵥λ�����(T_Manufacture_Dept.F_ID)
F_VerifyDept integer,               --�ͼ쵥λ�����(T_Verify_Dept.F_ID)
F_Grade smallint,                   --�����ȼ���1,2,3��
F_VerifyPerson integer,             --�춨Ա�����(T_User_Def_Tab.F_ID)
F_CheckPerson integer,              --����Ա�����(T_User_Def_Tab.F_ID)
F_DeviceInfoID integer,             --�춨װ���豸��ϢID�����(T_Verify_Device_Info.F_ID)
F_VerifyDate date,             	    --�춨����('2014-05-22')
F_ValidDate date,                   --�������Ч��(�����Ͳ���Ҫ)('2014-08-07')
F_EnvTemper float,                  --�����¶ȣ���λ��
F_EnvHumidity float,                --����ʪ��
F_AirPressure float,                --����ѹ��
F_CertNO varchar(16),               --�춨֤����(�����̲�д��ţ������û��Ժ���д)
F_FlowCoe float,                    --����ϵ��
F_WaterPress float,									--ˮѹ(MPa)
F_Q3 float,													--��������
F_Q3BiQ1 float,											--Q3/Q1
F_Q2BiQ1 float,											--Q2/Q1
F_Bak1 varchar(24),                 --������1
F_Bak2 varchar(24),                 --������2
F_Bak3 varchar(24),                 --������3
constraint F_Model_fk foreign key(F_Model) references T_Meter_Model(F_ID),
constraint F_Standard_fk foreign key(F_Standard) references T_Meter_Standard(F_ID),
constraint F_PickCode_fk foreign key(F_PickCode) references T_Meter_PickCode(F_ID),
constraint F_ManufactDept_fk foreign key(F_ManufactDept) references T_Manufacture_Dept(F_ID),
constraint F_VerifyDept_fk foreign key(F_VerifyDept) references T_Verify_Dept(F_ID),
constraint F_VerifyPerson_fk foreign key(F_VerifyPerson) references T_User_Def_Tab(F_ID),
constraint F_CheckPerson_fk foreign key(F_CheckPerson) references T_User_Def_Tab(F_ID),
constraint F_DeviceInfoID_fk foreign key(F_DeviceInfoID) references T_Verify_Device_Info(F_ID)
);
create unique index uk_T_Water_Verify_Record on T_Water_Verify_Record (F_MeterNo, F_TimeStamp, F_FlowPointIdx, F_MeterPosNo, F_DeviceInfoID);

