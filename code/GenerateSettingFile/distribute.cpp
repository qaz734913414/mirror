#include <QtCore\QtCore>

void testDistribute(const QString &inFile)
{
	qDebug() << "\r " << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QByteArray documentContent = file.readAll();
	file.close();

	QByteArray MD5arr = QCryptographicHash::hash(documentContent, QCryptographicHash::Md5).toHex();
	qDebug() << "MD5:" << MD5arr.data();

	QDataStream out(documentContent);

	quint32 mapID;
	qint32 need_lv, monsterCount, photo;
	QString name;
	QVector<quint32> vec_normal, vec_boss;
	bool bError = false;

	qint32 nCount = 0;
	while (!out.atEnd())
	{
		out >> mapID >> name >> photo >> need_lv >> monsterCount >> vec_normal >> vec_boss;
		++nCount;

		if (vec_normal.size() <= 0 || vec_boss.size() <=0)
		{
			bError = true;
			break;
		}
	}

	qDebug() << "has " << nCount << "map distribute define. the last define:";
	qDebug() << mapID << name << photo << need_lv << monsterCount << vec_normal.size() << vec_boss.size();
	if (bError)
	{
		qDebug() << "has error on reading map:" << mapID;
	}
}

void distribute(const QString &inFile, const QString &outFile)
{
	qDebug() << __FUNCTION__ << inFile << outFile;

	QFile Rfile(inFile);
	if (!Rfile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "open " << inFile << "error.";
		return;
	}

	QFile Wfile(outFile);
	if (!Wfile.open(QIODevice::WriteOnly))
	{
		qDebug() << "open " << outFile << "error.";
		return;
	}

	qint32 mapID, photo;
	qint32 need_lv, monsterCount, index, nStart, nStop, nCount = 0;
	QString name, strPath, strTmp, monster, boss;
	QStringList list, list1, list11;
	QVector<quint32> vec_normal, vec_boss;

	QDataStream iData(&Wfile);
	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		index = 0;
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		mapID = list.at(index++).toUInt();
		name = list.at(index++);
		photo = list.at(index++).toUInt();
		need_lv = list.at(index++).toInt();
		monsterCount = list.at(index++).toInt();

		vec_normal.clear();
		monster = list.at(index++);
		list1 = monster.split(",");
		for (qint32 i = 0; i < list1.size(); i++)
		{
			list11 = list1.at(i).split("-");
			nStart = list11.at(0).toUInt();
			nStop = list11.at(list11.size() - 1).toUInt();
			for (qint32 j = nStart; j <= nStop; j++)
			{
				vec_normal << j;
			}
		}

		vec_boss.clear();
		boss = list.at(index++);
		list1 = boss.split(",");
		for (qint32 i = 0; i < list1.size(); i++)
		{
			list11 = list1.at(i).split("-");
			nStart = list11.at(0).toUInt();
			nStop = list11.at(list11.size() - 1).toUInt();
			for (qint32 j = nStart; j <= nStop; j++)
			{
				vec_boss << j;
			}
		}
		iData << mapID << name << photo << need_lv << monsterCount << vec_normal << vec_boss;

		++nCount;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. has" << nCount << "maps";

	testDistribute(outFile);
}
