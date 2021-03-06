#ifndef _ROLEDEFINE_H
#define _ROLEDEFINE_H

const QString def_vocation[4] = {
	QStringLiteral("无职业"),
	QStringLiteral("战士"),
	QStringLiteral("法师"),
	QStringLiteral("道士")
};

const QString def_xiulian[12] = {
	QStringLiteral("凝气"),
	QStringLiteral("筑基"),
	QStringLiteral("结丹"),
	QStringLiteral("元婴"),
	QStringLiteral("化神"),
	QStringLiteral("婴变"),
	QStringLiteral("问鼎"),
	QStringLiteral("阴虚"),
	QStringLiteral("阳实"),
	QStringLiteral("窥涅"),
	QStringLiteral("净涅"),
	QStringLiteral("碎涅")
};

struct Info_jobAdd {
	qint32 level;		//等级
	qint32 exp;			//等级所需经验
	qint32 hp;			//血
	qint32 mp;			//蓝
	qint32 dc1;			//攻击_下限
	qint32 dc2;			//攻击_上限
	qint32 mc1;			//魔法
	qint32 mc2;			//魔法
	qint32 sc1;			//道术
	qint32 sc2;			//道术
	qint32 ac;			//物防
	qint32 mac;			//魔防
};
typedef QMap<qint32, QVector<Info_jobAdd>> mapJobAdd;

struct Info_Chenhao {
	qint32 level;
	QString name;
	qint32 need;
	qint32 dc1;
	qint32 dc2;
	qint32 mc1;
	qint32 mc2;
	qint32 sc1;
	qint32 sc2;
};

#endif //#ifndef _ROLEDEFINE_H

