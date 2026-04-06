.intel_syntax noprefix

.global AesKeyCaptureHookStub
.global g_AesHookStolenBytes

.extern g_AesCapturedKey
.extern g_AesHookResumeAddress

.section .rdata
g_AesHookStolenBytes: .long AesHookStolenBytesEnd - AesHookStolenBytesBegin

.text
AesKeyCaptureHookStub:
    push r10
    push r11

    sub rsp, 0x20
    movdqu [rsp], xmm0

    mov r10, rax
    movdqu xmm0, [r10]

    lea r11, [rip + g_AesCapturedKey]
    movdqu [r11], xmm0 # 16 bytes of the AES key are captured here

    movdqu xmm0, [rsp]
    add rsp, 0x20

    pop r11
    pop r10

AesHookStolenBytesBegin:
    pushfq
    push 0x3DBBFAAB
    push r12
    pop qword ptr [rsp]
    push rsi
    mov rsi, r12

AesHookStolenBytesEnd:
    mov rax, qword ptr [rip + g_AesHookResumeAddress]
    jmp rax
