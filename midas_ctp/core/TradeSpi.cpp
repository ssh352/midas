#include "TradeSpi.h"
#include "../helper/CtpVisualHelper.h"
#include "utils/log/Log.h"

using namespace std;

void TradeSpi::OnFrontConnected() {
    MIDAS_LOG_INFO("OnFrontConnected OK");
    manager->request_login();
}

void TradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo,
                              int nRequestID, bool bIsLast) {
    MIDAS_LOG_INFO("request ID " << nRequestID << " OnRspUserLogin: " << *pRspUserLogin);
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // save session parameters to test if my own order
        data->frontId = pRspUserLogin->FrontID;
        data->sessionId = pRspUserLogin->SessionID;
        int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
        iNextOrderRef++;
        sprintf(data->orderRef, "%d", iNextOrderRef);
        sprintf(data->execOrderRef, "%d", 1);
        sprintf(data->forQuoteRef, "%d", 1);
        sprintf(data->quoteRef, "%d", 1);

        MIDAS_LOG_INFO("current trading day = " << manager->api->GetTradingDay());
        ///投资者结算结果确认
        manager->request_confirm_settlement();
    }
}

void TradeSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
                                          CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    MIDAS_LOG_INFO("request ID " << nRequestID << " OnRspSettlementInfoConfirm " << *pSettlementInfoConfirm);
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // query_instrument("cu1712");
        manager->query_product("");
    }
}

void TradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo,
                                  int nRequestID, bool bIsLast) {
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " OnRspQryInstrument " << *pInstrument);
    data->instruments.insert(make_pair(string(pInstrument->InstrumentID), *pInstrument));
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // manager->query_trading_account();
    }
}

///请求查询交易所响应
void TradeSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                bool bIsLast) {
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " OnRspQryExchange " << *pExchange);
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // TODO record all info
    }
}

///请求查询产品响应
void TradeSpi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                               bool bIsLast) {
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " OnRspQryProduct " << *pProduct);
    data->products.insert(make_pair(string(pProduct->ProductID), *pProduct));
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // TODO record all info
    }
}

void TradeSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo,
                                      int nRequestID, bool bIsLast) {
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " OnRspQryTradingAccount " << *pTradingAccount);
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // query_position();
    }
}

void TradeSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition,
                                        CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (pInvestorPosition) {
        MIDAS_LOG_DEBUG("request ID " << nRequestID << " OnRspQryInvestorPosition " << *pInvestorPosition);
    } else {
        MIDAS_LOG_DEBUG("request ID " << nRequestID << " OnRspQryInvestorPosition ");
    }
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // 报单录入请求
        // request_insert_order();
        // 执行宣告录入请求
        // request_insert_execute_order();
        // 询价录入
        // ReqForQuoteInsert();
        // 做市商报价录入
        // ReqQuoteInsert();
    }
}

void TradeSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo,
                                int nRequestID, bool bIsLast) {
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " bIsLast " << bIsLast << " OnRspOrderInsert " << *pInputOrder);
    IsErrorRspInfo(pRspInfo);
}

void TradeSpi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo,
                                    int nRequestID, bool bIsLast) {
    //如果执行宣告正确，则不会进入该回调
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " bIsLast " << bIsLast << " OnRspExecOrderInsert "
                                  << *pInputExecOrder);
    IsErrorRspInfo(pRspInfo);
}

void TradeSpi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo,
                                   int nRequestID, bool bIsLast) {
    //如果询价正确，则不会进入该回调
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " bIsLast " << bIsLast << " OnRspForQuoteInsert "
                                  << *pInputForQuote);
    IsErrorRspInfo(pRspInfo);
}

void TradeSpi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo,
                                int nRequestID, bool bIsLast) {
    //如果报价正确，则不会进入该回调
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " bIsLast " << bIsLast << " OnRspQuoteInsert " << *pInputQuote);
    IsErrorRspInfo(pRspInfo);
}

void TradeSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo,
                                int nRequestID, bool bIsLast) {
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " bIsLast " << bIsLast << " OnRspOrderAction "
                                  << *pInputOrderAction);
    IsErrorRspInfo(pRspInfo);
}

void TradeSpi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInpuExectOrderAction,
                                    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    //正确的撤单操作，不会进入该回调
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " bIsLast " << bIsLast << " OnRspExecOrderAction "
                                  << *pInpuExectOrderAction);
    IsErrorRspInfo(pRspInfo);
}

void TradeSpi::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInpuQuoteAction, CThostFtdcRspInfoField *pRspInfo,
                                int nRequestID, bool bIsLast) {
    //正确的撤单操作，不会进入该回调
    MIDAS_LOG_DEBUG("request ID " << nRequestID << " bIsLast " << bIsLast << " OnRspQuoteAction " << *pInpuQuoteAction);
    IsErrorRspInfo(pRspInfo);
}

///报单通知
void TradeSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
    MIDAS_LOG_DEBUG("OnRtnOrder " << *pOrder);
    if (IsMyOrder(pOrder)) {
        if (IsTradingOrder(pOrder))
            manager->ReqOrderAction(pOrder);
        else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
            MIDAS_LOG_DEBUG("cancel order success");
    }
}

//执行宣告通知
void TradeSpi::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) {
    MIDAS_LOG_DEBUG("OnRtnExecOrder" << *pExecOrder);
    if (IsMyExecOrder(pExecOrder)) {
        if (IsTradingExecOrder(pExecOrder))
            manager->ReqExecOrderAction(pExecOrder);
        else if (pExecOrder->ExecResult == THOST_FTDC_OER_Canceled)
            MIDAS_LOG_DEBUG("执行宣告撤单成功");
    }
}

//询价通知
void TradeSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
    //上期所中金所询价通知通过该接口返回；只有做市商客户可以收到该通知
    MIDAS_LOG_DEBUG("OnRtnForQuoteRsp");
}

//报价通知
void TradeSpi::OnRtnQuote(CThostFtdcQuoteField *pQuote) {
    MIDAS_LOG_DEBUG("OnRtnQuote");
    if (IsMyQuote(pQuote)) {
        if (IsTradingQuote(pQuote))
            manager->ReqQuoteAction(pQuote);
        else if (pQuote->QuoteStatus == THOST_FTDC_OST_Canceled)
            MIDAS_LOG_DEBUG("quote cancel success");
    }
}

///成交通知
void TradeSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) { MIDAS_LOG_DEBUG("OnRtnTrade " << *pTrade); }

void TradeSpi::OnFrontDisconnected(int nReason) {
    MIDAS_LOG_DEBUG("OnFrontDisconnected");
    MIDAS_LOG_DEBUG("Reason = " << nReason);
}

void TradeSpi::OnHeartBeatWarning(int nTimeLapse) {
    MIDAS_LOG_DEBUG("OnHeartBeatWarning");
    MIDAS_LOG_DEBUG("nTimerLapse = " << nTimeLapse);
}

void TradeSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    MIDAS_LOG_DEBUG("OnRspError requestId: " << nRequestID << " isLast:" << bIsLast);
    IsErrorRspInfo(pRspInfo);
}

bool TradeSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo) {
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult) cerr << "ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << gbk2utf8(pRspInfo->ErrorMsg) << endl;
    return bResult;
}

bool TradeSpi::IsMyOrder(CThostFtdcOrderField *pOrder) {
    return ((pOrder->FrontID == data->frontId) && (pOrder->SessionID == data->sessionId) &&
            (strcmp(pOrder->OrderRef, data->orderRef) == 0));
}

bool TradeSpi::IsMyExecOrder(CThostFtdcExecOrderField *pExecOrder) {
    return ((pExecOrder->FrontID == data->frontId) && (pExecOrder->SessionID == data->sessionId) &&
            (strcmp(pExecOrder->ExecOrderRef, data->execOrderRef) == 0));
}

bool TradeSpi::IsMyQuote(CThostFtdcQuoteField *pQuote) {
    return ((pQuote->FrontID == data->frontId) && (pQuote->SessionID == data->sessionId) &&
            (strcmp(pQuote->QuoteRef, data->quoteRef) == 0));
}

bool TradeSpi::IsTradingOrder(CThostFtdcOrderField *pOrder) {
    return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
            (pOrder->OrderStatus != THOST_FTDC_OST_Canceled) && (pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

bool TradeSpi::IsTradingExecOrder(CThostFtdcExecOrderField *pExecOrder) {
    return (pExecOrder->ExecResult != THOST_FTDC_OER_Canceled);
}

bool TradeSpi::IsTradingQuote(CThostFtdcQuoteField *pQuote) { return (pQuote->QuoteStatus != THOST_FTDC_OST_Canceled); }

TradeSpi::TradeSpi(TradeManager *manager_, CtpData *d) : manager(manager_), data(d) {}
